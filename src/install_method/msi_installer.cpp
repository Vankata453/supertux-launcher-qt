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

#include "install_method/msi_installer.hpp"

#ifdef PLATFORM_WIN

namespace install_method {

MsiInstaller::MsiInstaller()
{
}

void
MsiInstaller::check_valid(const Instance& instance) const
{
  // TODO
}

TransferStatusListPtr
MsiInstaller::request_download(const Instance& instance) const
{
  // TODO
  return {};
}

InstanceConfigureInstall*
MsiInstaller::create_configure_install_dialog(const Instance& instance) const
{
  // TODO
  return nullptr;
}

QList<QProcess*>
MsiInstaller::create_install_processes(const Instance& instance, const InstanceConfigureInstall* config) const
{
  // TODO
  return {};
}

QProcess*
MsiInstaller::create_run_process(const Instance& instance) const
{
  // TODO
  return nullptr;
}

} // namespace install_method

#endif
