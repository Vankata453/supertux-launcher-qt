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

#include "dialog/instance_configure_install.hpp"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QSpinBox>

InstanceConfigureInstall::InstanceConfigureInstall() :
  QDialog(),
  m_layout(this),
  m_fields(),
  m_buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this))
{
  setWindowTitle("Configure Install");
  setWindowModality(Qt::WindowModal);
  setAttribute(Qt::WA_DeleteOnClose);

  // Add buttons
  m_layout.addRow(m_buttons);
  QObject::connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  QObject::connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


void
InstanceConfigureInstall::add_widget(int id, const std::string& label, QWidget* widget)
{
  assert(m_fields.find(id) == m_fields.end());

  m_layout.insertRow(m_layout.rowCount() - 1, QString::fromStdString(label) + ":", widget);
  m_fields[id] = widget;
}


QLineEdit*
InstanceConfigureInstall::add_textbox(int id, const std::string& label)
{
  QLineEdit* line_edit = new QLineEdit(this);

  add_widget(id, label, line_edit);
  return line_edit;
}

QComboBox*
InstanceConfigureInstall::add_combobox(int id, const std::string& label, const QStringList& entries)
{
  QComboBox* combo_box = new QComboBox(this);
  combo_box->addItems(entries);

  add_widget(id, label, combo_box);
  return combo_box;
}

QSpinBox*
InstanceConfigureInstall::add_spinbox(int id, const std::string& label, int minimum, int maximum)
{
  QSpinBox* spinbox = new QSpinBox(this);
  spinbox->setRange(minimum, maximum);

  add_widget(id, label, spinbox);
  return spinbox;
}


std::string
InstanceConfigureInstall::get_textbox_value(int id) const
{
  return get_widget<QLineEdit>(id)->text().toStdString();
}

int
InstanceConfigureInstall::get_combobox_value(int id) const
{
  const QComboBox* combobox = get_widget<QComboBox>(id);
  if (combobox->currentIndex() < 0)
    return -1;

  QVariant data = combobox->itemData(combobox->currentIndex());
  if (data.isValid())
    return data.toInt();

  return combobox->currentIndex();
}

int
InstanceConfigureInstall::get_spinbox_value(int id) const
{
  return get_widget<QSpinBox>(id)->value();
}
