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

#include "instance/instance.hpp"
#include "util/downloader.hpp"
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
  // TODO
}

TransferStatusListPtr
AppImage::install(Instance& instance) const
{
  const InstallMethod::Data& install_data = instance.m_version->m_install_methods.at(APPIMAGE);
  const std::string local_file = util::file_join(instance.get_install_directory().canonicalPath().toStdString(), INSTALL_FILENAME);

  TransferStatusPtr status = g_downloader.request_download(
      "https://github.com/" + install_data.repository + "/releases/download/" + install_data.tag + "/" + install_data.file,
      local_file
    );
  status->then([local_file](bool success)
    {
      if (!success)
        return;

      // Set executable permission for AppImage
      if (WEXITSTATUS(std::system(("chmod 755 " + local_file).c_str())) != 0)
        throw std::runtime_error("Error setting executable permission for \"" + local_file + "\"!");
    });

  return TransferStatusListPtr(new TransferStatusList({ status }));
}

int
AppImage::launch(const Instance& instance) const
{
  const std::string command = instance.m_version->get_run_command(
      util::file_join(instance.get_install_directory().canonicalPath().toStdString(), INSTALL_FILENAME),
      instance
    );
  return WEXITSTATUS(std::system(command.c_str()));
}

} // namespace install_method

#endif
