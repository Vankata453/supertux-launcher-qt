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

#include "instance/manager.hpp"

#include <algorithm>

static InstanceManager s_instance_manager;

InstanceManager::InstanceManager() :
  m_instances()
{
}

const Instance&
InstanceManager::get(const std::string& id)
{
  const auto it = std::find_if(m_instances.begin(), m_instances.end(), [id](const auto& instance)
    {
      return instance->m_id == id;
    });
  assert(it != m_instances.end()); // The instance must exist

  return **it;
}

void
InstanceManager::remove(const std::string& id)
{
  m_instances.erase(std::remove_if(m_instances.begin(), m_instances.end(),
                                   [id](const auto& instance)
                                   {
                                     return instance->m_id == id;
                                   }),
                    m_instances.end());

  MainWindow::current()->get_instance_list()->refresh();
}

bool
InstanceManager::exists(const std::string& id) const
{
  const auto it = std::find_if(m_instances.begin(), m_instances.end(), [id](const auto& instance)
    {
      return instance->m_id == id;
    });
  return it != m_instances.end();
}

std::vector<const Instance*>
InstanceManager::get_instances() const
{
  std::vector<const Instance*> result;

  for (const auto& instance : m_instances)
    result.push_back(instance.get());

  return result;
}
