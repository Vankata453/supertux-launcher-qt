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

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "instance/install_method.hpp"
#include "util/platform.hpp"

/** Represents a SuperTux version. */
class Version
{
public:
  enum Number
  {
    v0_6_3 = 0
  };
  static const std::vector<std::string> s_version_names;
  static const std::unordered_map<Number, const Version* const> s_versions;

public:
  Version();

  virtual Number get_number() const = 0;

  virtual std::vector<InstallMethod> get_install_methods() const = 0;

  const std::string& get_name() const;

private:
  Version(const Version&) = delete;
  Version& operator=(const Version&) = delete;
};
