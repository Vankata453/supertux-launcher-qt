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

#include "version/manager.hpp"

#include <iostream>

#include <QDir>

#include "util/platform.hpp"

VersionManager::VersionManager() :
  m_versions()
{
  // Load all available versions
  QDir versions_dir("data/versions");
  versions_dir.setNameFilters(QStringList("*.stlv")); // List only version files
  for (const QString& version_file : versions_dir.entryList(QDir::Files, QDir::Name))
  {
    try
    {
      m_versions.push_back(std::make_unique<Version>(versions_dir.filePath(version_file).toStdString()));
    }
    catch (const std::exception& err)
    {
      std::cout << "Couldn't load version file \"" << version_file.toStdString() << "\": " << err.what() << std::endl;
    }
  }
}

const Version*
VersionManager::get(int index) const
{
  return m_versions.at(index).get();
}

const Version*
VersionManager::get(const std::string& name) const
{
  const auto it = std::find_if(m_versions.begin(), m_versions.end(),
                               [name](const auto& version)
                               {
                                 return version->m_name == name;
                               });

  if (it == m_versions.end())
    throw std::runtime_error("No version with name \"" + name + "\"!");

  return it->get();
}

std::vector<const Version*>
VersionManager::get_versions() const
{
  std::vector<const Version*> result;

  for (const auto& version : m_versions)
    result.push_back(version.get());

  return result;
}

std::vector<std::string>
VersionManager::get_version_names() const
{
  std::vector<std::string> result;

  for (const auto& version : m_versions)
    result.push_back(version->m_name);

  return result;
}
