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

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "instance/manager.hpp"
#include "util/string.hpp"
#include "version/manager.hpp"

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
  QLineEdit* id_box = add_textbox(TextBox::ID, "ID");
  QLineEdit* name_box = add_textbox(TextBox::NAME, "Name");
  QComboBox* version_box = add_combobox(ComboBox::VERSION, "Version", {});

  QCheckBox* version_pre_release_box = new QCheckBox("Show pre-releases", this);
  m_layout.addRow("", version_pre_release_box);

  add_combobox(ComboBox::INSTALL_METHOD, "Install Method", {});

  QObject::connect(id_box, SIGNAL(textChanged(const QString&)), this, SLOT(on_id_modified()));
  QObject::connect(id_box, SIGNAL(textEdited(const QString&)), this, SLOT(on_id_modified_by_user()));

  name_box->setFocus();
  QObject::connect(name_box, SIGNAL(textChanged(const QString&)), this, SLOT(on_name_modified()));

  QObject::connect(version_pre_release_box, &QCheckBox::stateChanged, this, &AddInstanceDialog::on_pre_release_toggle);
  on_pre_release_toggle(Qt::Unchecked); // Initial

  QObject::connect(version_box, SIGNAL(currentIndexChanged(int)), this, SLOT(on_version_changed()));
  on_version_changed(); // Initial

  // Add buttons
  m_layout.addRow(m_buttons);
  QObject::connect(m_buttons, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));

  // Disable "OK" button, until an ID is provided
  m_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
}

QLineEdit*
AddInstanceDialog::add_textbox(TextBox id, const std::string& label)
{
  QLineEdit* line_edit = new QLineEdit(this);

  m_layout.addRow(QString::fromStdString(label) + ":", line_edit);
  m_textbox_fields[id] = line_edit;

  return line_edit;
}

QComboBox*
AddInstanceDialog::add_combobox(ComboBox id, const std::string& label, const QStringList& entries)
{
  QComboBox* combo_box = new QComboBox(this);
  combo_box->addItems(entries);

  m_layout.addRow(QString::fromStdString(label) + ":", combo_box);
  m_combobox_fields[id] = combo_box;

  return combo_box;
}

std::string
AddInstanceDialog::get_textbox_value(TextBox id) const
{
  const auto it = m_textbox_fields.find(id);
  assert(it != m_textbox_fields.end());

  return it->second->text().toStdString();
}

int
AddInstanceDialog::get_combobox_value(ComboBox id) const
{
  const auto it = m_combobox_fields.find(id);
  assert(it != m_combobox_fields.end());

  if (it->second->currentIndex() < 0)
    return -1;

  QVariant data = it->second->itemData(it->second->currentIndex());
  if (data.isValid())
    return data.toInt();

  return it->second->currentIndex();
}

void
AddInstanceDialog::update_ok_button()
{
  const QString id = m_textbox_fields[TextBox::ID]->text();

  // Enable the "OK" button only when:
  m_buttons->button(QDialogButtonBox::Ok)->setEnabled(
    m_combobox_fields[ComboBox::INSTALL_METHOD]->count() > 0 && // 1. The version has available installation methods.
    !id.isEmpty() && // 2. The ID field is not empty.
    !InstanceManager::current()->exists(id.toStdString()) // 3. An instance with the provided ID doesn't exist.
  );
}

void
AddInstanceDialog::on_id_modified()
{
  update_ok_button();
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
  m_textbox_fields[TextBox::ID]->setText(m_textbox_fields[TextBox::NAME]->text().replace(" ", "-"));
}

void
AddInstanceDialog::on_version_changed()
{
  QComboBox* install_method_box = m_combobox_fields[ComboBox::INSTALL_METHOD];

  const int version = get_combobox_value(ComboBox::VERSION);
  if (version < 0)
    return;

  // On version change, set the "Install Method" options to the ones specified for the new version
  install_method_box->clear();
  for (const auto& [method, _] : VersionManager::current()->get(version)->m_install_methods)
  {
    install_method_box->addItem(QString::fromStdString(InstallMethod::s_install_methods.at(method)->get_display_name()),
                                QVariant(static_cast<int>(method)));
  }

  // If no methods are available, disable the box
  install_method_box->setEnabled(install_method_box->count() > 0);

  update_ok_button();
}

void
AddInstanceDialog::on_pre_release_toggle(int check_state)
{
  QComboBox* version_box = m_combobox_fields[ComboBox::VERSION];
  const auto versions = VersionManager::current()->get_versions();

  version_box->clear();
  for (int i = 0; i < static_cast<int>(versions.size()); i++)
  {
    if (check_state != Qt::Checked && versions[i]->m_pre_release)
      continue;

    version_box->addItem(QString::fromStdString(versions[i]->m_name),
                         QVariant(i));
  }
}
