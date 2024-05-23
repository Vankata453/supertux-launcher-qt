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

#include <QTreeWidget>

class Instance;

class InstanceList final : public QTreeWidget
{
  Q_OBJECT;

public:
  InstanceList();

  void refresh();

  void paintEvent(QPaintEvent* event) override;

private:
  void push(const Instance& instance);

private Q_SLOTS:
  void on_selection_change() const;

public:
  class InstanceItem final : public QTreeWidgetItem
  {
  public:
    InstanceItem(const Instance& instance);

  public:
    const Instance& instance;

  private:
    InstanceItem(const InstanceItem&) = delete;
    InstanceItem& operator=(const InstanceItem&) = delete;
  };

  InstanceItem* get_selected_item() const;

private:
  InstanceList(const InstanceList&) = delete;
  InstanceList& operator=(const InstanceList&) = delete;
};
