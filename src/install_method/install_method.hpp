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

#include <QProcess>
#include <QStringList>

#include "util/downloader_defines.hpp"
#include "util/platform.hpp"

class Instance;
class InstanceConfigureInstall;
class ReaderMapping;

/** Represents a SuperTux instance install method.
    Install methods determine how an instance is installed and launched. */
class InstallMethod
{
public:
  enum Type
  {
#ifdef PLATFORM_WIN
    MSI_INSTALLER = 0,
    EXE_INSTALLER,
#elifdef PLATFORM_LINUX
    APPIMAGE = 0,
#endif
    ZIP_BINARY,
    SOURCE_BUILD
  };
  static const std::vector<std::string> s_install_method_names;
  static const std::unordered_map<Type, const InstallMethod* const> s_install_methods;

  static const InstallMethod* from_string(const std::string& method);

public:
  struct Data final
  {
  public:
    Data() = delete;
    Data(const ReaderMapping& mapping);

  public:
    std::string repository;
    std::string tag;
    std::string file;
  };

  class InstanceInvalidException final : public std::runtime_error
  {
  public:
    InstanceInvalidException(const std::string& message) : std::runtime_error(message) {}
  };

public:
  InstallMethod();

  virtual Type get_type() const = 0;

  const std::string& get_name() const;
  virtual std::string get_display_name() const = 0;

  /** Throws an InstanceInvalidException on failure. */
  virtual void check_valid(const Instance& instance) const = 0;

  virtual TransferStatusListPtr request_download(const Instance& instance) const = 0;

  virtual bool has_install_processes() const = 0;
  virtual InstanceConfigureInstall* create_configure_install_dialog(const Instance& instance) const = 0;
  virtual QList<QProcess*> create_install_processes(const Instance& instance, const InstanceConfigureInstall* config) const = 0;

  virtual QProcess* create_run_process(const Instance& instance) const = 0;

private:
  InstallMethod(const InstallMethod&) = delete;
  InstallMethod& operator=(const InstallMethod&) = delete;
};
