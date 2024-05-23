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

#include "version/v063.hpp"

namespace version {

v063::v063()
{
}

std::vector<InstallMethod>
v063::get_install_methods() const
{
  return {
#ifdef PLATFORM_WIN
    InstallMethod::MSI_INSTALLER
#elifdef PLATFORM_LINUX
    InstallMethod::APPIMAGE,
    InstallMethod::SOURCE_BUILD
#endif
  };
}

} // namespace version
