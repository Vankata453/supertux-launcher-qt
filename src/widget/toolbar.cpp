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

#include "widget/toolbar.hpp"

#include <iostream>
#include <sstream>

#include <QInputDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>

#include "dialog/add_instance.hpp"
#include "dialog/instance_install_dialog.hpp"
#include "instance/manager.hpp"
#include "widget/element/tool_button.hpp"
#include "widget/instance_list.hpp"
#include "window/main.hpp"

ToolBar::ToolBar() :
  QToolBar(),
  m_add_button(new ToolButton("Add", "data/images/icons/add.png", true)),
  m_remove_button(new ToolButton("Remove", "data/images/icons/remove.png", false)),
  m_clone_button(new ToolButton("Clone", "data/images/icons/clone.png", false)),
  m_options_button(new ToolButton("Options", "data/images/icons/options.png", false)),
  m_launch_button(new ToolButton("Launch", "data/images/icons/launch.png", false))
{
  setMovable(false);
  setFixedHeight(96);
  setIconSize(QSize(64, 64));

  // Add buttons
  addWidget(m_add_button);
  addWidget(m_remove_button);
  addSeparator();
  addWidget(m_clone_button);
  addWidget(m_options_button);
  addSeparator();
  addWidget(m_launch_button);

  // Create signal mappings
  connect(m_add_button, &QToolButton::clicked, this, &ToolBar::on_add_trigger);
  connect(m_remove_button, &QToolButton::clicked, this, &ToolBar::on_remove_trigger);
  connect(m_clone_button, &QToolButton::clicked, this, &ToolBar::on_clone_trigger);
  connect(m_options_button, &QToolButton::clicked, this, &ToolBar::on_options_trigger);
  connect(m_launch_button, &QToolButton::clicked, this, &ToolBar::on_launch_trigger);
}

void
ToolBar::on_add_trigger() const
{
  AddInstanceDialog add_dialog;
  if (add_dialog.exec() != QDialog::Accepted)
    return;

  const std::string id = add_dialog.get_textbox_value(AddInstanceDialog::TextBox::ID);
  const std::string name = add_dialog.get_textbox_value(AddInstanceDialog::TextBox::NAME);
  const int version = add_dialog.get_combobox_value(AddInstanceDialog::ComboBox::VERSION);
  const InstallMethod::Type method = static_cast<InstallMethod::Type>(add_dialog.get_combobox_value(AddInstanceDialog::ComboBox::INSTALL_METHOD));

  const Instance& instance = InstanceManager::current()->create(id, name, version, method);

  InstanceInstallDialog* install_dialog = new InstanceInstallDialog(MainWindow::current(), instance);
  install_dialog->start();
}

void
ToolBar::on_remove_trigger() const
{
  const Instance& instance = MainWindow::current()->get_instance_list()->get_selected_item()->instance;

  QMessageBox confirmation_box;
  confirmation_box.setWindowTitle("Remove Instance");
  confirmation_box.setText("You are about to delete the instance \"" + QString::fromStdString(instance.m_name) + "\".");
  confirmation_box.setInformativeText("Are you sure?");
  confirmation_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  confirmation_box.setDefaultButton(QMessageBox::No);
  switch (confirmation_box.exec())
  {
    case QMessageBox::Yes:
    {
      QMessageBox delete_data_box;
      delete_data_box.setWindowTitle("Remove Data Directory");
      delete_data_box.setText("Do you wish to erase the data directory for \"" + QString::fromStdString(instance.m_name) + "\" (ID: \"" + QString::fromStdString(instance.m_id) + "\")?");
      delete_data_box.setInformativeText("If you choose not to, a new instance with the same ID can use it.");
      delete_data_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
      delete_data_box.setDefaultButton(QMessageBox::Abort);

      const int result = delete_data_box.exec();
      switch (result)
      {
        // TODO
        case QMessageBox::Yes:
        case QMessageBox::No:
          try
          {
            InstanceManager::current()->remove(instance.m_id, result == QMessageBox::Yes);
          }
          catch (const std::exception& err)
          {
            std::stringstream out;
            out << "Couldn't remove instance \"" << instance.m_id << "\": " << err.what();
            std::cout << out.rdbuf() << std::endl;

            QMessageBox::critical(MainWindow::current(), "Error removing instance", QString::fromStdString(out.str()));
          }
          break;

        default:
          break;
      }
      break;
    }

    default:
      break;
  }
}

void
ToolBar::on_clone_trigger() const
{
  // TODO
}

void
ToolBar::on_options_trigger() const
{
  MainWindow::current()->show_instance_options(MainWindow::current()->get_instance_list()->get_selected_item()->instance);
}

void
ToolBar::on_launch_trigger() const
{
  InstanceManager::current()->launch(MainWindow::current()->get_instance_list()->get_selected_item()->instance.m_id);
}

void
ToolBar::toggle_instance_buttons(bool enabled)
{
  m_remove_button->setEnabled(enabled);
  m_clone_button->setEnabled(enabled);
  m_options_button->setEnabled(enabled);
  m_launch_button->setEnabled(enabled);
}
