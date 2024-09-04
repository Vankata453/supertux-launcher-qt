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

#include <ctime>
#include <string>

#include <QDateTime>
#include <QDir>

#include "install_method/install_method.hpp"

class Version;

class Instance final
{
  friend class InstanceManager;
  friend class InstanceProcessHandler;

public:
  Instance(const QDir& parent_dir, const std::string& id);
  Instance(const QDir& parent_dir, const std::string& id, const std::string& name,
           int version_idx, InstallMethod::Type install_method);

  void load();
  void save();

  inline bool is_running() const { return m_running; }

  QString get_build_log_filename() const;
  QString get_run_log_filename() const;

  const QDir& get_directory() const { return m_dir; }
  const QDir& get_install_directory() const { return m_install_dir; }
  const QDir& get_data_directory() const { return m_data_dir; }
  const QDir& get_logs_directory() const { return m_logs_dir; }
  const QDir& get_build_logs_directory() const { return m_build_logs_dir; }
  const QDir& get_run_logs_directory() const { return m_run_logs_dir; }

private:
  void initialize();

  void delete_directory();

  TransferStatusListPtr install();
  QProcess* create_process() const;

public:
  /* General info */
  const std::string m_id;
  std::string m_name;
  const Version* m_version;

  /* Additional info */
  QDateTime m_time_created;
  const InstallMethod* m_install_method;

private:
  /* Set by InstanceProcessHandler */
  bool m_running;

private:
  const QDir& m_parent_dir;
  QDir m_dir;
  QDir m_install_dir;
  QDir m_data_dir;
  QDir m_logs_dir;
  QDir m_build_logs_dir;
  QDir m_run_logs_dir;

private:
  Instance(const Instance&) = delete;
  Instance& operator=(const Instance&) = delete;
};
