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

#include "install_method/appimage.hpp"

#ifdef PLATFORM_LINUX

#include "dialog/instance_configure_install.hpp"
#include "instance/instance.hpp"
#include "util/downloader.hpp"
#include "util/qt.hpp"
#include "util/string.hpp"
#include "version/version.hpp"

static const std::string INSTALL_FILENAME = "install.AppImage";

namespace install_method {

AppImage::AppImage()
{
}

void
AppImage::check_valid(const Instance& instance) const
{
  QFileInfo install_file(QString::fromStdString(get_local_file(instance)));
  if (!install_file.exists() || !install_file.isFile())
    throw InstanceInvalidException("\"" + INSTALL_FILENAME + "\" is missing!");
}

TransferStatusListPtr
AppImage::request_download(const Instance& instance) const
{
  const InstallMethod::Data& install_data = instance.m_version->m_install_methods.at(APPIMAGE);

  TransferStatusPtr status = g_downloader.request_download(
      "https://github.com/" + install_data.repository + "/releases/download/" + install_data.tag + "/" + install_data.file,
      get_local_file(instance)
    );
  return TransferStatusListPtr(new TransferStatusList({ status }));
}

InstanceConfigureInstall*
AppImage::create_configure_install_dialog(const Instance&) const
{
  return nullptr;
}

QList<QProcess*>
AppImage::create_install_processes(const Instance& instance, const InstanceConfigureInstall*) const
{
  QList<QProcess*> processes;

  processes
    // Set executable permission for AppImage
    << util::qt::command_to_process("chmod -v 755 " + QString::fromStdString(get_local_file(instance)));

  return processes;
}

QProcess*
AppImage::create_run_process(const Instance& instance) const
{
  QProcess* process = new QProcess;
  process->setProgram(QString::fromStdString(get_local_file(instance)));
  process->setArguments(instance.m_version->get_run_arguments(instance));
  return process;
}


std::string
AppImage::get_local_file(const Instance& instance)
{
  return util::file_join(instance.get_install_directory().canonicalPath().toStdString(), INSTALL_FILENAME);
}

} // namespace install_method

#endif
