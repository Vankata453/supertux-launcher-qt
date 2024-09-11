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

#include <QDialog>
#include <QFormLayout>

class QComboBox;
class QDialogButtonBox;
class QLineEdit;
class QSpinBox;

class InstanceConfigureInstall final : public QDialog
{
public:
  InstanceConfigureInstall();

  std::string get_textbox_value(int id) const;
  int get_combobox_value(int id) const;
  int get_spinbox_value(int id) const;

  QLineEdit* add_textbox(int id, const std::string& label);
  QComboBox* add_combobox(int id, const std::string& label, const QStringList& entries);
  QSpinBox* add_spinbox(int id, const std::string& label, int minimum, int maximum);

  /* Add more fields if required by an InstallMethod. */

private:
  void add_widget(int id, const std::string& label, QWidget* widget);

  template<typename T>
  T* get_widget(int id) const
  {
    const auto it = m_fields.find(id);
    assert(it != m_fields.end());

    return static_cast<T*>(it->second);
  }

private:
  QFormLayout m_layout;

  std::unordered_map<int, QWidget*> m_fields;
  QDialogButtonBox* m_buttons;

private:
  InstanceConfigureInstall(const InstanceConfigureInstall&) = delete;
  InstanceConfigureInstall& operator=(const InstanceConfigureInstall&) = delete;
};
