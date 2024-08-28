//  SuperTux
//  Copyright (C) 2007 Christoph Sommer <christoph.sommer@2007.expires.deltadevelopment.de>
//                2014 Ingo Ruhnke <grumbel@gmail.com>
//                2023 Vankata453
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "util/downloader.hpp"

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "version.h"

#include "util/string.hpp"

Downloader g_downloader;

TransferStatus::TransferStatus(Downloader& downloader, TransferId id_,
                               const std::string& url) :
  m_downloader(downloader),
  id(id_),
  file(util::file_basename(url)),
  callbacks(),
  dltotal(0),
  dlnow(0),
  ultotal(0),
  ulnow(0),
  error_msg(),
  parent_list()
{}

void
TransferStatus::abort()
{
  m_downloader.abort(id);
}

void
TransferStatus::update()
{
  m_downloader.update();
}

TransferStatusList::TransferStatusList() :
  m_transfer_statuses(),
  m_successful_count(0),
  m_callbacks(),
  m_error_msg()
{
}

TransferStatusList::TransferStatusList(const std::vector<TransferStatusPtr>& list) :
  m_transfer_statuses(),
  m_successful_count(0),
  m_callbacks(),
  m_error_msg()
{
  for (TransferStatusPtr status : list)
  {
    push(status);
  }
}

void
TransferStatusList::abort()
{
  for (TransferStatusPtr status : m_transfer_statuses)
  {
    status->abort();
  }
  reset();
}

void
TransferStatusList::update()
{
  for (size_t i = 0; i < m_transfer_statuses.size(); i++)
  {
    m_transfer_statuses[i]->update();
  }
}

void
TransferStatusList::push(TransferStatusPtr status)
{
  status->parent_list = this;

  m_transfer_statuses.push_back(status);
}

void
TransferStatusList::push(TransferStatusListPtr statuses)
{
  for (TransferStatusPtr status : statuses->m_transfer_statuses)
  {
    push(status);
  }
}

// Called when one of the transfers completes.
void
TransferStatusList::on_transfer_complete(TransferStatusPtr this_status, bool successful)
{
  if (successful)
  {
    m_successful_count++;
    if (m_successful_count == static_cast<int>(m_transfer_statuses.size()))
    {
      // All transfers have sucessfully completed.
      bool success = true;
      for (const auto& callback : m_callbacks)
      {
        try
        {
          callback(success);
        }
        catch (const std::exception& err)
        {
          success = false;
          std::cout << "Exception in Downloader: TransferStatusList callback failed: " << err.what() << std::endl;
          m_error_msg = err.what();
        }
      }

      reset();
    }
  }
  else
  {
    std::stringstream err;
    err << "Downloading \"" << this_status->file << "\" failed: " << this_status->error_msg;
    m_error_msg = err.str();
    std::cout << "Exception in Downloader: TransferStatusList: " << m_error_msg << std::endl;

    // Execute all callbacks.
    for (const auto& callback : m_callbacks)
    {
      callback(false);
    }

    reset();
  }
}

int
TransferStatusList::get_download_now() const
{
  int dlnow = 0;
  for (TransferStatusPtr status : m_transfer_statuses)
  {
    dlnow += status->dlnow;
  }
  return dlnow;
}

int
TransferStatusList::get_download_total() const
{
  int dltotal = 0;
  for (TransferStatusPtr status : m_transfer_statuses)
  {
    dltotal += status->dltotal;
  }
  return dltotal;
}

void
TransferStatusList::reset()
{
  m_transfer_statuses.clear();
  m_callbacks.clear();
  m_successful_count = 0;
}

bool
TransferStatusList::is_active() const
{
  return !m_transfer_statuses.empty();
}

class Transfer
{
protected:
  Downloader& m_downloader;
  TransferId m_id;

  const std::string m_url;
  CURL* m_handle;
  std::array<char, CURL_ERROR_SIZE> m_error_buffer;

  TransferStatusPtr m_status;

public:
  Transfer(Downloader& downloader, TransferId id,
           const std::string& url) :
    m_downloader(downloader),
    m_id(id),
    m_url(url),
    m_handle(),
    m_error_buffer({{'\0'}}),
    m_status(new TransferStatus(m_downloader, id, url))
  {
    m_handle = curl_easy_init();
    if (!m_handle)
    {
      throw std::runtime_error("curl_easy_init() failed");
    }
    else
    {
      curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str());
      // cppcheck-suppress unknownMacro
      curl_easy_setopt(m_handle, CURLOPT_USERAGENT, "SuperTux Launcher/" PACKAGE_BUILD_HASH " libcURL");

      curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, this);
      curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, &Transfer::on_data_wrap);

      curl_easy_setopt(m_handle, CURLOPT_ERRORBUFFER, m_error_buffer.data());
      curl_easy_setopt(m_handle, CURLOPT_NOSIGNAL, 1);
      curl_easy_setopt(m_handle, CURLOPT_FAILONERROR, 1);
      curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1);

      curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, 0);
      curl_easy_setopt(m_handle, CURLOPT_PROGRESSDATA, this);
      curl_easy_setopt(m_handle, CURLOPT_PROGRESSFUNCTION, &Transfer::on_progress_wrap);
    }
  }

  virtual ~Transfer()
  {
    curl_easy_cleanup(m_handle);
  }

  TransferStatusPtr get_status() const
  {
    return m_status;
  }

  const char* get_error_buffer() const
  {
    return m_error_buffer.data();
  }

  TransferId get_id() const
  {
    return m_id;
  }

  CURL* get_curl_handle() const
  {
    return m_handle;
  }

  const std::string& get_url() const
  {
    return m_url;
  }

  virtual size_t on_data(const char* ptr, size_t size, size_t nmemb) = 0;

  int on_progress(double dltotal, double dlnow,
                   double ultotal, double ulnow)
  {
    m_status->dltotal = static_cast<int>(dltotal);
    m_status->dlnow = static_cast<int>(dlnow);

    m_status->ultotal = static_cast<int>(ultotal);
    m_status->ulnow = static_cast<int>(ulnow);

    return 0;
  }

private:
  static size_t on_data_wrap(const char* ptr, size_t size, size_t nmemb, void* userdata)
  {
    return static_cast<Transfer*>(userdata)->on_data(ptr, size, nmemb);
  }

  static int on_progress_wrap(void* userdata,
                              double dltotal, double dlnow,
                              double ultotal, double ulnow)
  {
    return static_cast<Transfer*>(userdata)->on_progress(dltotal, dlnow, ultotal, ulnow);
  }

private:
  Transfer(const Transfer&) = delete;
  Transfer& operator=(const Transfer&) = delete;
};

class FileTransfer final : public Transfer
{
private:
  std::ofstream m_fout;

public:
  FileTransfer(Downloader& downloader, TransferId id,
               const std::string& url, const std::string& outfile) :
    Transfer(downloader, id, url),
    m_fout()
  {
    m_fout.open(outfile, std::ios::out | std::ios::binary);
  }

  ~FileTransfer() override
  {
    m_fout.close();
  }

  size_t on_data(const char* ptr, size_t size, size_t nmemb) override
  {
    m_fout.write(ptr, size * nmemb);
    return size * nmemb;
  }

private:
  FileTransfer(const FileTransfer&) = delete;
  FileTransfer& operator=(const FileTransfer&) = delete;
};

class StringTransfer final : public Transfer
{
private:
  std::string& m_out;

public:
  StringTransfer(Downloader& downloader, TransferId id,
                 const std::string& url, std::string& outstr) :
    Transfer(downloader, id, url),
    m_out(outstr)
  {
  }

  size_t on_data(const char* ptr, size_t size, size_t nmemb) override
  {
    m_out += std::string(ptr, size * nmemb);
    return size * nmemb;
  }

private:
  StringTransfer(const StringTransfer&) = delete;
  StringTransfer& operator=(const StringTransfer&) = delete;
};

Downloader::Downloader() :
  m_multi_handle(),
  m_transfers(),
  m_next_transfer_id(1)
{
  curl_global_init(CURL_GLOBAL_ALL);
  m_multi_handle = curl_multi_init();
  if (!m_multi_handle)
  {
    throw std::runtime_error("curl_multi_init() failed");
  }
}

Downloader::~Downloader()
{
  for (auto& transfer : m_transfers)
  {
    curl_multi_remove_handle(m_multi_handle, transfer.second->get_curl_handle());
  }
  m_transfers.clear();

  curl_multi_cleanup(m_multi_handle);
  curl_global_cleanup();
}

void
Downloader::abort(TransferId id)
{
  auto it = m_transfers.find(id);
  if (it == m_transfers.end())
  {
    std::cout << "transfer not found: " << id << std::endl;
  }
  else
  {
    TransferStatusPtr status = (it->second)->get_status();

    curl_multi_remove_handle(m_multi_handle, it->second->get_curl_handle());
    m_transfers.erase(it);

    for (const auto& callback : status->callbacks)
    {
      try
      {
        callback(false);
      }
      catch(const std::exception& err)
      {
        std::cout << "Illegal exception in Downloader: " << err.what() << std::endl;
      }
    }
    if (status->parent_list)
      status->parent_list->on_transfer_complete(status, false);
  }
}

void
Downloader::update()
{
  // Read data from the network.
  CURLMcode ret;
  int running_handles;
  while ((ret = curl_multi_perform(m_multi_handle, &running_handles)) == CURLM_CALL_MULTI_PERFORM)
  {
  }

  // Check if any downloads got finished.
  int msgs_in_queue;
  CURLMsg* msg;
  while ((msg = curl_multi_info_read(m_multi_handle, &msgs_in_queue)))
  {
    switch (msg->msg)
    {
      case CURLMSG_DONE:
        {
          CURLcode resultfromcurl = msg->data.result;
          curl_multi_remove_handle(m_multi_handle, msg->easy_handle);

          auto it = std::find_if(m_transfers.begin(), m_transfers.end(),
                                 [&msg](const auto& rhs) {
                                   return rhs.second->get_curl_handle() == msg->easy_handle;
                                 });
          assert(it != m_transfers.end());
          TransferStatusPtr status = it->second->get_status();
          status->error_msg = it->second->get_error_buffer();
          m_transfers.erase(it);

          if (resultfromcurl == CURLE_OK)
          {
            bool success = true;
            for (const auto& callback : status->callbacks)
            {
              try
              {
                callback(success);
              }
              catch(const std::exception& err)
              {
                success = false;
                std::cout << "Exception in Downloader: " << err.what() << std::endl;
                status->error_msg = err.what();
              }
            }
            if (status->parent_list)
              status->parent_list->on_transfer_complete(status, success);
          }
          else
          {
            std::cout << "Error: " << curl_easy_strerror(resultfromcurl) << std::endl;
            for (const auto& callback : status->callbacks)
            {
              try
              {
                callback(false);
              }
              catch(const std::exception& err)
              {
                std::cout << "Illegal exception in Downloader: " << err.what() << std::endl;
              }
            }
            if (status->parent_list)
              status->parent_list->on_transfer_complete(status, false);
          }
        }
        break;

      default:
        std::cout << "Unhandled cURL message: " << msg->msg << std::endl;
        break;
    }
  }
}

TransferStatusPtr
Downloader::add_transfer(std::unique_ptr<Transfer> transfer)
{
  curl_multi_add_handle(m_multi_handle, transfer->get_curl_handle());
  auto transferId = transfer->get_id();
  m_transfers[transferId] = std::move(transfer);
  return m_transfers[transferId]->get_status();
}

TransferStatusPtr
Downloader::request_download(const std::string& url, const std::string& filename)
{
  return add_transfer(std::make_unique<FileTransfer>(*this, m_next_transfer_id++, url, filename));
}

TransferStatusPtr
Downloader::request_string_download(const std::string& url, std::string& out_string)
{
  return add_transfer(std::make_unique<StringTransfer>(*this, m_next_transfer_id++, url, out_string));
}

/* EOF */
