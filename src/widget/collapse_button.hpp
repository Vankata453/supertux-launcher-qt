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

#include <QParallelAnimationGroup>
#include <QToolButton>

class QPropertyAnimation;

class CollapseButton : public QToolButton
{
public:
  CollapseButton(QWidget* parent, int max_content_height = 50, int animation_duration = 300);

  void set_text(const QString& text);
  void set_content(QWidget* content);
  void set_max_content_height(int height) { m_max_content_height = height; }
  void set_animation_duration(int duration) { m_animation_duration = duration; }

private:
  void on_toggle(bool checked);

private:
  int m_max_content_height;
  int m_animation_duration;

  QWidget* m_content;
  QParallelAnimationGroup m_animator;
  QPropertyAnimation* m_animation;
  QPropertyAnimation* m_parent_animation;

private:
  CollapseButton(const CollapseButton&) = delete;
  CollapseButton& operator=(const CollapseButton&) = delete;
};
