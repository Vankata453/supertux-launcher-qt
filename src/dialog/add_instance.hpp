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
#include <unordered_map>
#include <vector>

#include <QDialog>
#include <QFormLayout>

class QComboBox;
class QDialogButtonBox;
class QLineEdit;

class AddInstanceDialog final : public QDialog
{
  Q_OBJECT;

public:
  enum class TextBox
  {
    ID,
    NAME
  };
  enum class ComboBox
  {
    VERSION,
    INSTALL_METHOD
  };

public:
  AddInstanceDialog();

  std::string get_textbox_value(TextBox id) const;
  int get_combobox_value(ComboBox id) const;

private:
  QLineEdit* add_textbox(TextBox id, const std::string& label);
  QComboBox* add_combobox(ComboBox id, const std::string& label, const QStringList& entries);

  void update_ok_button();

private Q_SLOTS:
  void on_id_modified();
  void on_id_modified_by_user();
  void on_name_modified();
  void on_version_changed();
  void on_pre_release_toggle(int check_state);

private:
  QFormLayout m_layout;

  std::unordered_map<TextBox, QLineEdit*> m_textbox_fields;
  std::unordered_map<ComboBox, QComboBox*> m_combobox_fields;
  QDialogButtonBox* m_buttons;

  bool m_lock_id;

private:
  AddInstanceDialog(const AddInstanceDialog&) = delete;
  AddInstanceDialog& operator=(const AddInstanceDialog&) = delete;
};
