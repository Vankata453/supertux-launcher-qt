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

#include "util/platform.hpp"

#ifdef PLATFORM_WIN

#include "install_method/install_method.hpp"

namespace install_method {

class ExeInstaller final : public InstallMethod
{
public:
  ExeInstaller();

  Type get_type() const override { return EXE_INSTALLER; }

  std::string get_display_name() const override { return "EXE Installer"; }

  void check_valid(const Instance& instance) const override;

  TransferStatusListPtr install(Instance& instance) const override;
  QProcess* create_process(const Instance& instance) const override;

private:
  ExeInstaller(const ExeInstaller&) = delete;
  ExeInstaller& operator=(const ExeInstaller&) = delete;
};

} // namespace install_method

#endif
