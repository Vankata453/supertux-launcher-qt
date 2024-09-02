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
#include <iostream>

#include "util/string.hpp"
#include "version/version.hpp"

InstanceManager::InstanceItem::InstanceItem(const Instance& instance_) :
  QStandardItem(QString::fromStdString(instance_.m_name)),
  instance(instance_)
{
}


InstanceManager::InstanceManager() :
  QStandardItemModel(),
  m_instances(),
  m_instances_dir(QDir("instances"))
{
  // Initialize model columns
  setHorizontalHeaderItem(0, new QStandardItem("Name"));
  setHorizontalHeaderItem(1, new QStandardItem("Version"));
  setHorizontalHeaderItem(2, new QStandardItem("Created"));

  // Create "instances" directory, if it doesn't exist
  if (!m_instances_dir.exists() && !QDir::current().mkdir("instances"))
    throw std::runtime_error("Error creating \"instances\" directory!");

  // Load all instances
  for (const QString& instance_dir : m_instances_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name))
  {
    try
    {
      auto instance = std::make_unique<Instance>(m_instances_dir, instance_dir.toStdString()); // The directory name represents the ID
      append_instance_item(*instance);
      m_instances.push_back(std::move(instance));
    }
    catch (const std::exception& err)
    {
      std::cout << "Couldn't load instance \"" << instance_dir.toStdString() << "\": " << err.what() << std::endl;
    }
  }
}

InstanceManager::~InstanceManager()
{
  // Save all instances
  for (const auto& instance : m_instances)
    instance->save();
}

Instance&
InstanceManager::get_modifiable(const std::string& id) const
{
  const auto it = std::find_if(m_instances.begin(), m_instances.end(), [id](const auto& instance)
    {
      return instance->m_id == id;
    });
  assert(it != m_instances.end()); // The instance must exist

  return **it;
}

const Instance&
InstanceManager::get(const std::string& id) const
{
  return get_modifiable(id);
}

void
InstanceManager::remove(const std::string& id) // TODO: With/without saves
{
  remove_instance_item(id);

  m_instances.erase(std::remove_if(m_instances.begin(), m_instances.end(),
                                   [id](const auto& instance)
                                   {
                                     if (instance->m_id == id)
                                     {
                                       instance->delete_directory();
                                       return true;
                                     }
                                     return false;
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

TransferStatusListPtr
InstanceManager::install(const std::string& id)
{
  Instance& instance = get_modifiable(id);
  try
  {
    return instance.install();
  }
  catch (const std::exception& err)
  {
    std::cout << "Couldn't install instance \"" + id + "\": " << err.what() << std::endl;
    return {};
  }
}

bool
InstanceManager::launch(const std::string& id) const
{
  const Instance& instance = get(id);
  try
  {
    return instance.launch();
  }
  catch (const std::exception& err)
  {
    std::cout << "Couldn't launch instance \"" + id + "\": " << err.what() << std::endl;
    return false;
  }
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
      new QStandardItem(QString::fromStdString(instance.m_version->m_name)),
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
