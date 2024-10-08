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

#include "instance/instance.hpp"

#include <iostream>

#include <QTimeZone>

#include "util/reader_document.hpp"
#include "util/reader_mapping.hpp"
#include "util/writer.hpp"
#include "version/manager.hpp"

const QString Instance::s_data_filename = "data.stlid";

Instance::Instance(const QDir& parent_dir, const std::string& id) :
  m_id(id),
  m_name("<unnamed>"),
  m_version(),
  m_time_created(),
  m_install_method(),
  m_running(false),
  m_parent_dir(parent_dir),
  m_dir(parent_dir.filePath(QString::fromStdString(id))),
  m_install_dir(m_dir.filePath("install")),
  m_data_dir(m_dir.filePath("data")),
  m_logs_dir(m_dir.filePath("logs")),
  m_build_logs_dir(m_logs_dir.filePath("build")),
  m_run_logs_dir(m_logs_dir.filePath("run"))
{
  if (!m_install_dir.exists())
    throw std::runtime_error("No \"install\" directory!");
  if (!m_data_dir.exists())
    throw std::runtime_error("No \"data\" directory!");
  if (!m_logs_dir.exists() && !m_dir.mkdir("logs"))
    throw std::runtime_error("Error creating \"logs\" directory for instance \"" + m_id + "\"!");
  if (!m_build_logs_dir.exists() && !m_logs_dir.mkdir("build"))
    throw std::runtime_error("Error creating \"logs/build\" directory for instance \"" + m_id + "\"!");
  if (!m_run_logs_dir.exists() && !m_logs_dir.mkdir("run"))
    throw std::runtime_error("Error creating \"logs/run\" directory for instance \"" + m_id + "\"!");

  initialize();
  load();

  try
  {
    m_install_method->check_valid(*this);
  }
  catch (const InstallMethod::InstanceInvalidException& err)
  {
    throw std::runtime_error(std::string("Instance is invalid: ") + err.what());
  }
}

Instance::Instance(const QDir& parent_dir, const std::string& id, const std::string& name,
                   int version_idx, InstallMethod::Type install_method) :
  m_id(id),
  m_name(name),
  m_version(VersionManager::current()->get(version_idx)),
  m_time_created(QDateTime::currentDateTime()), // Current time
  m_install_method(InstallMethod::s_install_methods.at(install_method)),
  m_running(false),
  m_parent_dir(parent_dir),
  m_dir(parent_dir.filePath(QString::fromStdString(id))),
  m_install_dir(m_dir.filePath("install")),
  m_data_dir(m_dir.filePath("data")),
  m_logs_dir(m_dir.filePath("logs")),
  m_build_logs_dir(m_logs_dir.filePath("build")),
  m_run_logs_dir(m_logs_dir.filePath("run"))
{
  initialize();
  save();

  if (!m_install_dir.exists() && !m_dir.mkdir("install"))
    throw std::runtime_error("Error creating \"install\" directory for instance \"" + m_id + "\"!");
  if (!m_data_dir.exists() && !m_dir.mkdir("data"))
    throw std::runtime_error("Error creating \"data\" directory for instance \"" + m_id + "\"!");
  if (!m_logs_dir.exists() && !m_dir.mkdir("logs"))
    throw std::runtime_error("Error creating \"logs\" directory for instance \"" + m_id + "\"!");
  if (!m_build_logs_dir.exists() && !m_logs_dir.mkdir("build"))
    throw std::runtime_error("Error creating \"logs/build\" directory for instance \"" + m_id + "\"!");
  if (!m_run_logs_dir.exists() && !m_logs_dir.mkdir("run"))
    throw std::runtime_error("Error creating \"logs/run\" directory for instance \"" + m_id + "\"!");
}

void
Instance::initialize()
{
  m_build_logs_dir.setNameFilters({ "*.log" });
  m_run_logs_dir.setNameFilters({ "*.log" });
}

void
Instance::load()
{
  try
  {
    auto doc = ReaderDocument::from_file(m_dir.filePath(s_data_filename).toStdString());
    auto root = doc.get_root();
    if (root.get_name() != "supertux-launcher-instance")
      throw std::runtime_error("File is not a \"supertux-launcher-instance\" file!");

    auto mapping = root.get_mapping();

    mapping.get("name", m_name);
    if (m_name.empty())
      throw std::runtime_error("No instance name specified!");

    std::string version_name;
    mapping.get("version", version_name);
    m_version = VersionManager::current()->get(version_name);

    int time_created;
    mapping.get("created-time", time_created);
    m_time_created = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(time_created));

    std::string install_method;
    mapping.get("install-method", install_method);
    m_install_method = InstallMethod::from_string(install_method);
  }
  catch (const std::exception& err)
  {
    throw std::runtime_error(std::string("Couldn't read instance data file (\"data.stlid\"): ") + err.what());
  }
}

void
Instance::save()
{
  // Create instance directory, if it doesn't exist
  if (!m_dir.exists() && !m_parent_dir.mkdir(QString::fromStdString(m_id)))
    throw std::runtime_error("Error creating directory for instance \"" + m_id + "\"!");

  // Save instance data file
  Writer writer(m_dir.filePath(s_data_filename).toStdString());
  writer.start_list("supertux-launcher-instance");

  writer.write("name", m_name);
  writer.write("version", m_version->m_name);
  writer.write("created-time", static_cast<int>(m_time_created.toSecsSinceEpoch()));
  writer.write("install-method", m_install_method->get_name());

  writer.end_list("supertux-launcher-instance");
}

void
Instance::delete_directory(bool with_data)
{
  if (with_data)
  {
    m_dir.removeRecursively();
    return;
  }

  // Remove everything, except the data folder
  for (const QString file : m_dir.entryList(QDir::Files))
  {
    m_dir.remove(file);
  }
  m_install_dir.removeRecursively();
  m_logs_dir.removeRecursively();
  m_build_logs_dir.removeRecursively();
  m_run_logs_dir.removeRecursively();
}

TransferStatusListPtr
Instance::request_download() const
{
  return m_install_method->request_download(*this);
}

bool
Instance::has_install_processes() const
{
  return m_install_method->has_install_processes();
}

InstanceConfigureInstall*
Instance::create_configure_install_dialog() const
{
  return m_install_method->create_configure_install_dialog(*this);
}

QList<QProcess*>
Instance::create_install_processes(const InstanceConfigureInstall* config) const
{
  return m_install_method->create_install_processes(*this, config);
}

QProcess*
Instance::create_run_process() const
{
  return m_install_method->create_run_process(*this);
}

QString
Instance::get_build_log_filename() const
{
  return m_build_logs_dir.filePath(QString::number(QDateTime::currentSecsSinceEpoch()) + ".log");
}

QString
Instance::get_run_log_filename() const
{
  return m_run_logs_dir.filePath(QString::number(QDateTime::currentSecsSinceEpoch()) + ".log");
}
