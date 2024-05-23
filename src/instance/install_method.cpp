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

#include "instance/install_method.hpp"

std::string InstallMethod_to_string(InstallMethod method)
{
  switch (method)
  {
    case InstallMethod::MSI_INSTALLER:
      return "msi";
    case InstallMethod::EXE_INSTALLER:
      return "exe";
    case InstallMethod::ZIP_BINARY:
      return "zip";
    case InstallMethod::APPIMAGE:
      return "appimage";
    case InstallMethod::SOURCE_BUILD:
      return "source-build";
    default:
      return "unknown";
  }
}

InstallMethod InstallMethod_from_string(const std::string& method)
{
  if (method == "msi")
    return InstallMethod::MSI_INSTALLER;
  if (method == "exe")
    return InstallMethod::EXE_INSTALLER;
  if (method == "zip")
    return InstallMethod::ZIP_BINARY;
  if (method == "appimage")
    return InstallMethod::APPIMAGE;
  if (method == "source-build")
    return InstallMethod::SOURCE_BUILD;

  return InstallMethod::UNKNOWN;
}

QString InstallMethod_to_display_string(InstallMethod method)
{
  switch (method)
  {
    case InstallMethod::MSI_INSTALLER:
      return "MSI Installer";
    case InstallMethod::EXE_INSTALLER:
      return "EXE Installer";
    case InstallMethod::ZIP_BINARY:
      return "ZIP Binary";
    case InstallMethod::APPIMAGE:
      return "AppImage";
    case InstallMethod::SOURCE_BUILD:
      return "Source Build";
    default:
      return "Unknown";
  }
}

QStringList InstallMethods_to_display_strings(const std::vector<InstallMethod>& methods)
{
  QStringList strings;

  strings.reserve(methods.size());
  std::transform(methods.begin(), methods.end(), std::back_inserter(strings),
                 [](const InstallMethod& method)
                 {
                   return InstallMethod_to_display_string(method);
                 });

  return strings;
}
