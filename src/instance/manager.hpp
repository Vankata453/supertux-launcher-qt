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

#include "util/currenton.hpp"

#include <memory>
#include <vector>

#include <QDir>
#include <QStandardItemModel>

#include "instance/instance.hpp"

/** A manager for SuperTux instances. */
class InstanceManager final : public Currenton<InstanceManager>,
                              public QStandardItemModel
{
public:
  InstanceManager();
  virtual ~InstanceManager();

  const Instance& get(const std::string& id) const;

  template<typename... Args>
  const Instance& create(Args&&... args)
  {
    auto instance = std::make_unique<Instance>(m_instances_dir, std::forward<Args>(args)...);

    assert(!exists(instance->m_id)); // The instance must not exist

    const Instance& instance_ref = *instance;
    append_instance_item(instance_ref);

    m_instances.push_back(std::move(instance));
    return instance_ref;
  }
  void remove(const std::string& id);

  bool exists(const std::string& id) const;

  TransferStatusListPtr install(const std::string& id);
  void launch(const std::string& id);

  std::vector<const Instance*> get_instances() const;

public:
  class InstanceItem final : public QStandardItem
  {
  public:
    InstanceItem(const Instance& instance);

  public:
    const Instance& instance;

  private:
    InstanceItem(const InstanceItem&) = delete;
    InstanceItem& operator=(const InstanceItem&) = delete;
  };

private:
  Instance& get_modifiable(const std::string& id) const;

  void append_instance_item(const Instance& instance);
  void remove_instance_item(const std::string& id);

private:
  std::vector<std::unique_ptr<Instance>> m_instances;
  QDir m_instances_dir;

private:
  InstanceManager(const InstanceManager&) = delete;
  InstanceManager& operator=(const InstanceManager&) = delete;
};
