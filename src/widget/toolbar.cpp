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

#include <QInputDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>

#include "dialog/add_instance.hpp"
#include "widget/instance_list.hpp"
#include "window/main.hpp"

ToolBar::ToolButton::ToolButton(const std::string& text, const std::string& icon, bool enabled) :
  QToolButton()
{
  resize(96, 96);
  setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

  setIcon(QIcon(QString::fromStdString(icon)));
  setText(QString::fromStdString(text));
  setEnabled(enabled);
}


ToolBar::ToolBar() :
  QToolBar(),
  m_add_button(new ToolButton("Add", "data/images/icons/add.png", true)),
  m_remove_button(new ToolButton("Remove", "data/images/icons/remove.png", false)),
  m_clone_button(new ToolButton("Clone", "data/images/icons/clone.png", false)),
  m_options_button(new ToolButton("Options", "data/images/icons/options.png", false))
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

  // Create signal mappings
  connect(m_add_button, SIGNAL(clicked()), this, SLOT(on_add_trigger()));
  connect(m_remove_button, SIGNAL(clicked()), this, SLOT(on_remove_trigger()));
  connect(m_clone_button, SIGNAL(clicked()), this, SLOT(on_clone_trigger()));
  connect(m_options_button, SIGNAL(clicked()), this, SLOT(on_options_trigger()));
}

void
ToolBar::on_add_trigger() const
{
  AddInstanceDialog dialog;
  if (dialog.exec() != QDialog::Accepted)
    return;

  MainWindow::current()->get_instance_list()->push(dialog.get_textbox_value(AddInstanceDialog::Field::ID),
                                                   dialog.get_textbox_value(AddInstanceDialog::Field::NAME),
                                                   dialog.get_textbox_value(AddInstanceDialog::Field::VERSION),
                                                   "21.05.2024");
}

void
ToolBar::on_remove_trigger() const
{
  const std::string& instance_name = MainWindow::current()->get_instance_list()->get_selected_item()->get_name();

  QMessageBox confirmation_box;
  confirmation_box.setWindowTitle("Remove Instance");
  confirmation_box.setText("You are about to delete the instance \"" + QString::fromStdString(instance_name) + "\".");
  confirmation_box.setInformativeText("Are you sure?");
  confirmation_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  confirmation_box.setDefaultButton(QMessageBox::No);
  switch (confirmation_box.exec())
  {
    case QMessageBox::Yes:
    {
      const std::string& instance_id = MainWindow::current()->get_instance_list()->get_selected_item()->get_id();

      QMessageBox delete_data_box;
      delete_data_box.setWindowTitle("Remove Data Directory");
      delete_data_box.setText("Do you wish to erase the data directory for \"" + QString::fromStdString(instance_name) + "\" (ID: \"" + QString::fromStdString(instance_id) + "\")?");
      delete_data_box.setInformativeText("If you choose not to, a new instance with the same ID can use it.");
      delete_data_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
      delete_data_box.setDefaultButton(QMessageBox::Abort);
      switch (delete_data_box.exec())
      {
        // TODO
        case QMessageBox::Yes:
        case QMessageBox::No:
          MainWindow::current()->get_instance_list()->pop_selected();
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
  // TODO
}

void
ToolBar::toggle_instance_buttons(bool enabled)
{
  m_remove_button->setEnabled(enabled);
  m_clone_button->setEnabled(enabled);
  m_options_button->setEnabled(enabled);
}
