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
#include <QSortFilterProxyModel>

#include "widget/toolbar.hpp"
#include "window/main.hpp"

InstanceList::InstanceList() :
  QTreeView(),
  m_proxy_model(new QSortFilterProxyModel())
{
  setRootIsDecorated(false);

  // Set model
  m_proxy_model->setSourceModel(InstanceManager::current());
  m_proxy_model->sort(2); // Sort by "Created" date
  m_proxy_model->setSortRole(Qt::UserRole);
  setModel(m_proxy_model);

  // Set column properties
  setColumnWidth(0, 300); // "Name"
  setColumnWidth(1, 150); // "Version"

  // Create signal mappings
  QObject::connect(selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(on_selection_change()));
}

void
InstanceList::paintEvent(QPaintEvent* event)
{
  QTreeView::paintEvent(event);

  if (model()->rowCount(rootIndex()) > 0)
    return;

  // The list is empty, show a text indicating that
  QPainter painter(viewport());
  painter.drawText(rect(), Qt::AlignCenter, "No instances available!\n\nPress \"Add\" to create an instance.");
}

void
InstanceList::on_selection_change() const
{
  MainWindow::current()->get_toolbar()->toggle_instance_buttons(!selectedIndexes().empty());
}

InstanceManager::InstanceItem*
InstanceList::get_selected_item() const
{
  QModelIndexList indexes = selectedIndexes();
  if (indexes.empty())
    return nullptr;

  QModelIndex index = m_proxy_model->mapToSource(indexes[0]);
  return static_cast<InstanceManager::InstanceItem*>(InstanceManager::current()->item(index.row()));
}
