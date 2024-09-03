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

#include <QToolBar>

class Instance;
class LogBox;
class QComboBox;
class QMainWindow;
class QToolButton;
class WidgetToolButton;

class InstanceOptionsBar final : public QToolBar
{
  Q_OBJECT;

public:
  InstanceOptionsBar(QMainWindow* window, Instance& instance);

private Q_SLOTS:
  void on_trigger();

private:
  QMainWindow* m_window;
  QToolButton* m_active_button;
  LogBox* m_log_box;
  QComboBox* m_log_select;

  WidgetToolButton* m_log_button;

private:
  InstanceOptionsBar(const InstanceOptionsBar&) = delete;
  InstanceOptionsBar& operator=(const InstanceOptionsBar&) = delete;
};
