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

#include "window/main.hpp"

#include <QApplication>
#include <QLabel>
#include <QScreen>
#include <QStatusBar>

#include "version.h"

#include "widget/instance_list.hpp"
#include "widget/toolbar.hpp"

MainWindow::MainWindow() :
  QMainWindow(),
  m_toolbar(new ToolBar),
  m_instance_list(new InstanceList)
{
  setWindowTitle("SuperTux Launcher");
  resize(800, 600);

  // Center window on screen
  move(screen()->geometry().center() - frameGeometry().center());

  // Add widgets
  setMenuWidget(m_toolbar);
  setCentralWidget(m_instance_list);

  // Set up status bar
  QLabel* version_label = new QLabel("Nightly Build: " + QString(PACKAGE_BUILD_HASH));
  statusBar()->addPermanentWidget(version_label);
}
