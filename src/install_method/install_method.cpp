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

#include "install_method/install_method.hpp"

#include <algorithm>
#include <iostream>

#include "util/reader_mapping.hpp"

/* Install methods */
#include "install_method/appimage.hpp"
#include "install_method/exe_installer.hpp"
#include "install_method/msi_installer.hpp"
#include "install_method/source_build.hpp"
#include "install_method/zip_binary.hpp"

const std::vector<std::string> InstallMethod::s_install_method_names = {
#ifdef PLATFORM_WIN
  "msi",
  "exe",
#elifdef PLATFORM_LINUX
  "appimage",
#endif
  "zip",
  "source-build"
};

const std::unordered_map<InstallMethod::Type, const InstallMethod* const> InstallMethod::s_install_methods = {
#ifdef PLATFORM_WIN
  { MSI_INSTALLER, new install_method::MsiInstaller() },
  { EXE_INSTALLER, new install_method::ExeInstaller() },
#elifdef PLATFORM_LINUX
  { APPIMAGE, new install_method::AppImage() },
#endif
  { ZIP_BINARY, new install_method::ZipBinary() },
  { SOURCE_BUILD, new install_method::SourceBuild() }
};

const InstallMethod*
InstallMethod::from_string(const std::string& name)
{
  const auto it = std::find(s_install_method_names.begin(), s_install_method_names.end(), name);
  if (it == s_install_method_names.end())
    throw std::runtime_error("No install method with name \"" + name + "\"!");

  return s_install_methods.at(static_cast<Type>(it - s_install_method_names.begin()));
}


InstallMethod::Data::Data(const ReaderMapping& mapping) :
  repository(),
  tag(),
  file()
{
  mapping.get("repository", repository);
  mapping.get("tag", tag);
  mapping.get("file", file);
}


InstallMethod::InstallMethod()
{
}

const std::string&
InstallMethod::get_name() const
{
  return s_install_method_names.at(get_type());
}
