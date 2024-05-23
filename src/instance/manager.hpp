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

#include "instance/instance.hpp"
#include "widget/instance_list.hpp"
#include "window/main.hpp"

/** A manager for SuperTux instances. */
class InstanceManager final : public Currenton<InstanceManager>
{
public:
  InstanceManager();

  const Instance& get(const std::string& id);

  template<typename... Args>
  const Instance& create(Args&&... args)
  {
    auto instance = std::make_unique<Instance>(std::forward<Args>(args)...);

    assert(!exists(instance->m_id)); // The instance must not exist

    m_instances.push_back(std::move(instance));
    MainWindow::current()->get_instance_list()->refresh();

    return *m_instances.back();
  }
  void remove(const std::string& id);

  bool exists(const std::string& id) const;

  std::vector<const Instance*> get_instances() const;

private:
  std::vector<std::unique_ptr<Instance>> m_instances;

private:
  InstanceManager(const InstanceManager&) = delete;
  InstanceManager& operator=(const InstanceManager&) = delete;
};
