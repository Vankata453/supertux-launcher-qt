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

#include "widget/options_bar.hpp"

#include <QMainWindow>

#include "widget/element/widget_tool_button.hpp"
#include "widget/log_box.hpp"

OptionsBar::OptionsBar(QMainWindow* window) :
  QToolBar(),
  m_active_button(),
  m_window(window),
  m_log_button(new WidgetToolButton("Log", "data/images/icons/add.png", true, new LogBox(window))) // TODO: Proper log icon
{
  setMovable(false);
  setFixedHeight(96);
  setIconSize(QSize(64, 64));

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
OptionsBar::on_trigger()
{
  WidgetToolButton* button = static_cast<WidgetToolButton*>(sender());

  m_active_button->setDown(false);
  m_active_button = button;
  button->setDown(true);

  assert(button->get_widget());
  m_window->setCentralWidget(button->get_widget());
}
