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

#include "window/options_instance.hpp"

#include <QScreen>

#include "instance/instance.hpp"
#include "widget/instance_options_bar.hpp"

OptionsInstance::OptionsInstance(Instance& instance) :
  QMainWindow(),
  m_instance(instance),
  m_options_bar(new InstanceOptionsBar(this, instance))
{
  setWindowTitle("Options: " + QString::fromStdString(m_instance.m_name));
  resize(600, 400);

  // Center window on screen
  move(screen()->geometry().center() - frameGeometry().center());

  // Add widgets
  setMenuWidget(m_options_bar);
}
