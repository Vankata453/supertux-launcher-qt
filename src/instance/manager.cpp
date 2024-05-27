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

#include "util/string.hpp"
#include "version/version.hpp"

InstanceManager::InstanceItem::InstanceItem(const Instance& instance_) :
  QStandardItem(QString::fromStdString(instance_.m_name)),
  instance(instance_)
{
}


static InstanceManager s_instance_manager;

InstanceManager::InstanceManager() :
  QStandardItemModel(),
  m_instances()
{
  setHorizontalHeaderItem(0, new QStandardItem("Name"));
  setHorizontalHeaderItem(1, new QStandardItem("Version"));
  setHorizontalHeaderItem(2, new QStandardItem("Created"));
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
  remove_instance_item(id);

  m_instances.erase(std::remove_if(m_instances.begin(), m_instances.end(),
                                   [id](const auto& instance)
                                   {
                                     return instance->m_id == id;
                                   }),
                    m_instances.end());
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

void
InstanceManager::append_instance_item(const Instance& instance)
{
  QStandardItem* time_created = new QStandardItem(instance.m_time_created.toString("MMMM dd, yyyy hh:mm:ss"));
  time_created->setData(instance.m_time_created, Qt::UserRole);

  appendRow({
      new InstanceItem(instance),
      new QStandardItem(QString::fromStdString(instance.m_version->get_name())),
      time_created
    });
}

void
InstanceManager::remove_instance_item(const std::string& id)
{
  for (int i = 0; i < rowCount(); i++)
  {
    const InstanceItem* instance_item = static_cast<const InstanceItem*>(item(i));
    if (instance_item->instance.m_id == id)
    {
      removeRow(instance_item->row());
      break;
    }
  }
}
