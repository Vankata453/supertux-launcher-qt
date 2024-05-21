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

#include "widget/toolbar.hpp"
#include "window/main.hpp"

InstanceList::InstanceItem::InstanceItem(const std::string& id) :
  m_id(id)
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
}

void
InstanceList::push(const std::string& id, const std::string& name, const std::string& version, const std::string& created_time)
{
  for (int i = 0; i < topLevelItemCount(); i++)
  {
    InstanceItem* item = static_cast<InstanceItem*>(topLevelItem(i));
    if (item->get_id() == id) // Item with given ID already exists
      return;
  }

  InstanceItem* item = new InstanceItem(id);

  item->setText(0, QString::fromStdString(name));
  item->setText(1, QString::fromStdString(version));
  item->setText(2, QString::fromStdString(created_time));

  addTopLevelItem(item);
}

void
InstanceList::pop(const std::string& id)
{
  for (int i = 0; i < topLevelItemCount(); i++)
  {
    InstanceItem* item = static_cast<InstanceItem*>(topLevelItem(i));
    if (item->get_id() == id) // Match!
    {
      delete item;
      break;
    }
  }
}

void
InstanceList::pop_selected()
{
  for (QTreeWidgetItem* item : selectedItems())
    delete item;

  setCurrentItem(nullptr);
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
