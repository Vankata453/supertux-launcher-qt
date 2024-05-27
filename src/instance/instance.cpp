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

#include "instance/instance.hpp"

Instance::Instance(const std::string& id) :
  m_id(id),
  m_name("<unnamed>"),
  m_version(),
  m_time_created(),
  m_install_method()
{
  // TODO: Load from data file
}

Instance::Instance(const std::string& id, const std::string& name,
                   Version::Number version, InstallMethod install_method) :
  m_id(id),
  m_name(name),
  m_version(Version::s_versions.at(version)),
  m_time_created(QDateTime::currentDateTime()), // Current time
  m_install_method(install_method)
{
}
