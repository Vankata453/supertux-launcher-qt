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
#include <map>

#include "install_method/install_method.hpp"

class Instance;

/** Stores parsed data about a SuperTux version. */
class Version final
{
public:
  Version(const std::string& file);

  std::string get_run_command(const std::string& path, const Instance& instance) const;

public:
  std::string m_name;
  std::string m_run_format;
  std::map<InstallMethod::Type, InstallMethod::Data> m_install_methods;

private:
  Version(const Version&) = delete;
  Version& operator=(const Version&) = delete;
};
