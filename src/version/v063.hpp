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

#include "version/version.hpp"

namespace version {

class v063 final : public Version
{
public:
  v063();

  Number get_number() const override { return v0_6_3; }

  std::vector<InstallMethod> get_install_methods() const override;

private:
  v063(const v063&) = delete;
  v063& operator=(const v063&) = delete;
};

} // namespace version
