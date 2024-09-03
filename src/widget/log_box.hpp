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

#include <QPlainTextEdit>

class QWidget;

class LogBox final : public QPlainTextEdit
{
  Q_OBJECT;

public:
  LogBox(QWidget* parent);

  void set_file(const QString& file);

public Q_SLOTS:
  void on_file_change();

private:
  LogBox(const LogBox&) = delete;
  LogBox& operator=(const LogBox&) = delete;
};
