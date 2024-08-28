//  SuperTux Launcher
//  Copyright (C) 2024 Vankata453
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

#include "dialog/download_dialog.hpp"

#include "util/downloader.hpp"

DownloadDialog::DownloadDialog(const std::string& title, const std::string& success_title,
                               TransferStatusPtr status) :
  QProgressDialog(QString::fromStdString(title), "Cancel", 0, 100),
  m_status(new TransferStatusList({ status })),
  m_success_title(success_title)
{
}

DownloadDialog::DownloadDialog(const std::string& title, const std::string& success_title,
                               TransferStatusListPtr status) :
  QProgressDialog(QString::fromStdString(title), "Cancel", 0, 100),
  m_status(status),
  m_success_title(success_title)
{
}
 
void
DownloadDialog::start()
{
  setWindowModality(Qt::WindowModal);
  setAutoClose(false);
  show();

  while (m_status->is_active())
  {
    if (wasCanceled())
    {
      m_status->abort();

      close();
      return;
    }

    m_status->update();

    setValue(m_status->get_download_now());
    setMaximum(m_status->get_download_total());
  }

  setLabelText(QString::fromStdString(m_status->get_error().empty() ? m_success_title : m_status->get_error()));
  setCancelButtonText("Close");
}
