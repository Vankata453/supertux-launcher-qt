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

#include "widget/log_box.hpp"

#include <QComboBox>
#include <QFile>
#include <QTextStream>

LogBox::LogBox(QWidget* parent) :
  QPlainTextEdit(parent)
{
  setReadOnly(true);
  setPlainText("No logs available!");
}

void
LogBox::on_file_change(int)
{
  QComboBox* box = static_cast<QComboBox*>(sender());

  const QVariant data = box->itemData(box->currentIndex());
  assert(data.isValid());

  set_file(data.toString());
}

void
LogBox::set_file(const QString& file)
{
  QFile log(file);
  if (!log.open(QFile::ReadOnly | QFile::Text))
  {
    setPlainText("ERROR: Unable to open \"" + file + "\" for read!");
    return;
  }
  QTextStream log_in(&log);
  setPlainText(log_in.readAll());
}
