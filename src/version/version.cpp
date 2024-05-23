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

#include "version/version.hpp"

/* Versions */
#include "version/v063.hpp"

const std::vector<std::string> Version::s_version_names = {
  "v0.6.3"
};

const std::unordered_map<Version::Number, const Version* const> Version::s_versions = {
  { v0_6_3, new version::v063() }
};


Version::Version()
{
}

const std::string&
Version::get_name() const
{
  return s_version_names.at(get_number());
}
