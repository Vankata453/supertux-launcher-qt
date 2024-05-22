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

#include "dialog/add_instance.hpp"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

AddInstanceDialog::AddInstanceDialog() :
  QDialog(),
  m_layout(this),
  m_textbox_fields(),
  m_combobox_fields(),
  m_buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this)),
  m_lock_id(false)
{
  setWindowTitle("Add Instance");

  // Add textboxes
  QLineEdit* id_box = add_textbox(Field::ID, "ID");
  QLineEdit* name_box = add_textbox(Field::NAME, "Name");
  add_combobox(Field::VERSION, "Version", { "0.6.3", "0.6.2", "0.6.1", "0.6.0" /*...*/ }); // TODO
  add_combobox(Field::INSTALL_METHOD, "Install Method", { "Compile From Source", "MSI installer" /*...*/ }); // TODO

  QObject::connect(id_box, SIGNAL(textChanged(const QString&)), this, SLOT(on_id_modified()));
  QObject::connect(id_box, SIGNAL(textEdited(const QString&)), this, SLOT(on_id_modified_by_user()));

  name_box->setFocus();
  QObject::connect(name_box, SIGNAL(textChanged(const QString&)), this, SLOT(on_name_modified()));

  // Add buttons
  m_layout.addRow(m_buttons);
  QObject::connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));

  // Disable "OK" button, until an ID is provided
  m_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
}

QLineEdit*
AddInstanceDialog::add_textbox(Field id, const std::string& label)
{
  QLineEdit* line_edit = new QLineEdit(this);

  m_layout.addRow(QString::fromStdString(label) + ":", line_edit);
  m_textbox_fields[id] = line_edit;

  return line_edit;
}

QComboBox*
AddInstanceDialog::add_combobox(Field id, const std::string& label, const QStringList& entries)
{
  QComboBox* combo_box = new QComboBox(this);
  combo_box->addItems(entries);

  m_layout.addRow(QString::fromStdString(label) + ":", combo_box);
  m_combobox_fields[id] = combo_box;

  return combo_box;
}

std::string
AddInstanceDialog::get_textbox_value(Field id) const
{
  {
    const auto it = m_textbox_fields.find(id);
    if (it != m_textbox_fields.end())
      return it->second->text().toStdString();
  }
  {
    const auto it = m_combobox_fields.find(id);
    assert(it != m_combobox_fields.end());

    return it->second->currentText().toStdString();
  }
}

void
AddInstanceDialog::on_id_modified()
{
  // Toggle "OK" button, depending on whether the ID field is empty
  m_buttons->button(QDialogButtonBox::Ok)->setEnabled(!m_textbox_fields[Field::ID]->text().isEmpty());
}

void
AddInstanceDialog::on_id_modified_by_user()
{
  // If the user edits the ID once, lock it, so changing the name wouldn't automatically edit it
  m_lock_id = true;
}

void
AddInstanceDialog::on_name_modified()
{
  if (m_lock_id)
    return;

  // Automatically generate an ID from the new name
  // TODO: Handle illegal filename chars
  m_textbox_fields[Field::ID]->setText(m_textbox_fields[Field::NAME]->text().replace(" ", "-"));
}
