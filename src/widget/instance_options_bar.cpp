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

#include "widget/instance_options_bar.hpp"

#include <QComboBox>
#include <QMainWindow>
#include <QTimeZone>

#include "instance/instance.hpp"
#include "widget/element/widget_tool_button.hpp"
#include "widget/form_layout.hpp"
#include "widget/log_box.hpp"

InstanceOptionsBar::InstanceOptionsBar(QMainWindow* window, Instance& instance) :
  QToolBar(window),
  m_active_button(),
  m_window(window),
  m_log_box(new LogBox(this)),
  m_log_select(new QComboBox(this)),
  m_log_button(new WidgetToolButton("Log", "data/images/icons/add.png", true, // TODO: Proper log icon
                                    new FormLayout(this, { m_log_box, m_log_select })))
{
  setMovable(false);
  setFixedHeight(96);
  setIconSize(QSize(64, 64));

  // Set up: LOG
  const QStringList build_logs = instance.get_build_logs_directory().entryList(QDir::Files, QDir::Name);
  const QStringList run_logs = instance.get_run_logs_directory().entryList(QDir::Files, QDir::Name);
  if (build_logs.count() == 0 && run_logs.count() == 0)
  {
    m_log_select->setEnabled(false);
  }
  else
  {
    for (const QString& log_file : build_logs)
    {
      const QDateTime log_time = QDateTime::fromSecsSinceEpoch(QFileInfo(log_file).baseName().toLongLong(), QTimeZone::utc());
      m_log_select->insertItem(0, log_time.toString("MMMM dd, yyyy hh:mm:ss") + " [BUILD]", QVariant(instance.get_build_logs_directory().filePath(log_file))); 
    }
    for (const QString& log_file : run_logs)
    {
      const QDateTime log_time = QDateTime::fromSecsSinceEpoch(QFileInfo(log_file).baseName().toLongLong(), QTimeZone::utc());
      m_log_select->insertItem(0, log_time.toString("MMMM dd, yyyy hh:mm:ss"), QVariant(instance.get_run_logs_directory().filePath(log_file))); 
    }

    QObject::connect(m_log_select, SIGNAL(currentIndexChanged(int)), m_log_box, SLOT(on_file_change()));
    if (m_log_select->currentIndex() == 0)
      m_log_box->set_file(m_log_select->itemData(0).toString());
    else
      m_log_select->setCurrentIndex(0);
  }

  // Add buttons
  addWidget(m_log_button);

  // Create signal mappings
  connect(m_log_button, SIGNAL(clicked()), this, SLOT(on_trigger()));

  // Set first button as active
  m_active_button = m_log_button;
  m_log_button->setDown(true);
  m_window->setCentralWidget(m_log_button->get_widget());
}

void
InstanceOptionsBar::on_trigger()
{
  WidgetToolButton* button = static_cast<WidgetToolButton*>(sender());

  m_active_button->setDown(false);
  m_active_button = button;
  button->setDown(true);

  assert(button->get_widget());
  m_window->setCentralWidget(button->get_widget());
}
