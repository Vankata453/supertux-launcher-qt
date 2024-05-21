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
#include <QDesktopWidget>
#include <QLabel>
#include <QScreen>
#include <QStatusBar>

#include "version.h"

#include "widget/instance_list.hpp"
#include "widget/toolbar.hpp"

MainWindow* MainWindow::s_current = nullptr;

MainWindow::MainWindow() :
  QMainWindow(),
  m_toolbar(new ToolBar),
  m_instance_list(new InstanceList)
{
  assert(!s_current);
  s_current = this;

  setWindowTitle("SuperTux Launcher");
  resize(800, 600);

  // Center window on screen
  const QRect screen_rect = QGuiApplication::primaryScreen()->geometry();
  move((screen_rect.width() - width()) / 2,
       (screen_rect.height() - height()) / 2);

  // Add widgets
  setMenuWidget(m_toolbar);
  setCentralWidget(m_instance_list);

  // Set up status bar
  QLabel* version_label = new QLabel("Nightly Build: " + QString(PACKAGE_BUILD_HASH));
  statusBar()->addPermanentWidget(version_label);
}

MainWindow::~MainWindow()
{
  assert(s_current);
  s_current = nullptr;
}
