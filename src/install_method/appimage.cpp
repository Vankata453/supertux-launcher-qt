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

#include "instance/instance.hpp"
#include "util/downloader.hpp"
#include "util/string.hpp"
#include "version/version.hpp"

#ifdef PLATFORM_LINUX

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

  TransferStatusPtr status = g_downloader.request_download(
    "https://github.com/" + install_data.repository + "/releases/download/" + install_data.tag + "/" + install_data.file,
    util::file_join(instance.get_install_directory().canonicalPath().toStdString(), install_data.file)
  );

  return TransferStatusListPtr(new TransferStatusList({ status }));
}

void
AppImage::launch(Instance& instance) const
{
  // TODO
}

} // namespace install_method

#endif
