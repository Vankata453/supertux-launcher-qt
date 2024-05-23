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

#include "widget/instance_list.hpp"

#include <QPainter>

#include "instance/manager.hpp"
#include "util/string.hpp"
#include "version/version.hpp"
#include "widget/toolbar.hpp"
#include "window/main.hpp"

InstanceList::InstanceItem::InstanceItem(const Instance& instance_) :
  instance(instance_)
{
}


InstanceList::InstanceList() :
  QTreeWidget()
{
  setRootIsDecorated(false);

  // Set columns
  QStringList columns;
  columns << "Name" << "Version" << "Created";
  setHeaderLabels(columns);

  setColumnWidth(0, 300); // "Name"
  setColumnWidth(1, 150); // "Version"

  // Create signal mappings
  connect(this, SIGNAL(itemSelectionChanged()), SLOT(on_selection_change()));

  // Load items
  refresh();
}

void
InstanceList::refresh()
{
  clear();

  // Re-add all instances as items
  for (const Instance* instance : InstanceManager::current()->get_instances())
    push(*instance);
}

void
InstanceList::push(const Instance& instance)
{
  InstanceItem* item = new InstanceItem(instance);

  item->setText(0, QString::fromStdString(instance.m_name));
  item->setText(1, QString::fromStdString(instance.m_version->get_name()));
  item->setText(2, QString::fromStdString(util::timestamp_to_date_string(instance.m_time_created, "%B %d, %Y %T")));

  addTopLevelItem(item);
}

void
InstanceList::paintEvent(QPaintEvent* event)
{
  QTreeWidget::paintEvent(event);

  if (model()->rowCount(rootIndex()) > 0)
    return;

  // The list is empty, show a text indicating that
  QPainter painter(viewport());
  painter.drawText(rect(), Qt::AlignCenter, "No instances available!\n\nPress \"Add\" to create an instance.");
}

void
InstanceList::on_selection_change() const
{
  MainWindow::current()->get_toolbar()->toggle_instance_buttons(!selectedItems().empty());
}

InstanceList::InstanceItem*
InstanceList::get_selected_item() const
{
  return currentItem() ? static_cast<InstanceItem*>(currentItem()) : nullptr;
}
