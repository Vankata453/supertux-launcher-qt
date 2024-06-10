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
#include <unordered_map>
#include <vector>

#include <QStringList>

#include "util/platform.hpp"

/** Represents a SuperTux instance install method.
    Install methods provide functions for installing and launching an instance. (TODO) */
class InstallMethod
{
public:
  enum Type
  {
    UNKNOWN = 0,
#ifdef PLATFORM_WIN
    MSI_INSTALLER,
    EXE_INSTALLER,
#endif
    ZIP_BINARY,
#ifdef PLATFORM_LINUX
    APPIMAGE,
#endif
    SOURCE_BUILD
  };
  static const std::vector<std::string> s_install_method_names;
  static const std::unordered_map<Type, const InstallMethod* const> s_install_methods;

  static const InstallMethod* from_string(const std::string& method);
  static QStringList to_display_names(const std::vector<InstallMethod::Type>& methods);

public:
  InstallMethod();

  virtual Type get_type() const = 0;

  const std::string& get_name() const;
  virtual std::string get_display_name() const = 0;

private:
  InstallMethod(const InstallMethod&) = delete;
  InstallMethod& operator=(const InstallMethod&) = delete;
};
