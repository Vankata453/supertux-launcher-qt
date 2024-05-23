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

#pragma once

#include <string>

#include <QStringList>

enum class InstallMethod
{
  UNKNOWN,
  MSI_INSTALLER,
  EXE_INSTALLER,
  ZIP_BINARY,
  APPIMAGE,
  SOURCE_BUILD
};

std::string InstallMethod_to_string(InstallMethod method);
InstallMethod InstallMethod_from_string(const std::string& method);

QString InstallMethod_to_display_string(InstallMethod method);
QStringList InstallMethods_to_display_strings(const std::vector<InstallMethod>& methods);
