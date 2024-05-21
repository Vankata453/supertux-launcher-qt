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

#include <QToolBar>
#include <QToolButton>

class QEvent;

class ToolBar final : public QToolBar
{
  Q_OBJECT;

public:
  ToolBar();

  void toggle_instance_buttons(bool enabled);

private Q_SLOTS:
  void on_add_trigger() const;
  void on_remove_trigger() const;
  void on_clone_trigger() const;
  void on_options_trigger() const;

private:
  class ToolButton final : public QToolButton
  {
  public:
    ToolButton(const std::string& text, const std::string& icon, bool enabled);

  private:
    ToolButton(const ToolButton&) = delete;
    ToolButton& operator=(const ToolButton&) = delete;
  };

private:
  ToolButton* m_add_button;
  ToolButton* m_remove_button;
  ToolButton* m_clone_button;
  ToolButton* m_options_button;

private:
  ToolBar(const ToolBar&) = delete;
  ToolBar& operator=(const ToolBar&) = delete;
};
