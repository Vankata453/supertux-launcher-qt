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

#ifdef PLATFORM_LINUX

#include "install_method/install_method.hpp"

namespace install_method {

class AppImage final : public InstallMethod
{
public:
  AppImage();

  Type get_type() const override { return APPIMAGE; }

  std::string get_display_name() const override { return "AppImage"; }

  void check_valid(const Instance& instance) const override;

  TransferStatusListPtr request_download(const Instance& instance) const override;

  bool has_install_processes() const override { return true; }
  InstanceConfigureInstall* create_configure_install_dialog(const Instance& instance) const override;
  QList<QProcess*> create_install_processes(const Instance& instance, const InstanceConfigureInstall* config) const override;

  QProcess* create_run_process(const Instance& instance) const override;

private:
  static std::string get_local_file(const Instance& instance);

private:
  AppImage(const AppImage&) = delete;
  AppImage& operator=(const AppImage&) = delete;
};

} // namespace install_method

#endif
