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

#include <QTreeView>

#include "instance/manager.hpp"

class QSortFilterProxyModel;

class InstanceList final : public QTreeView
{
public:
  InstanceList();

  void paintEvent(QPaintEvent* event) override;

  InstanceManager::InstanceItem* get_selected_item() const;

private:
  void on_selection_change(const QItemSelection&, const QItemSelection&) const;

private:
  QSortFilterProxyModel* m_proxy_model;

private:
  InstanceList(const InstanceList&) = delete;
  InstanceList& operator=(const InstanceList&) = delete;
};
