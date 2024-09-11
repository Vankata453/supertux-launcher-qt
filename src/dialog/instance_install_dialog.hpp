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

#include <QDialog>
#include <QFile>
#include <QFormLayout>
#include <QProcess>
#include <QTextStream>

#include "util/downloader_defines.hpp"

class CollapseButton;
class Instance;
class InstanceConfigureInstall;
class QDialogButtonBox;
class QLabel;
class QPlainTextEdit;
class QProgressBar;
class QTimer;

class InstanceInstallDialog final : public QDialog
{
public:
  InstanceInstallDialog(QWidget* parent, const Instance& instance);

  void start();

private:
  void on_install_process_error(const QProcess* process, QProcess::ProcessError err);
  void on_install_process_finish(QList<QProcess*>::iterator it, int exit_code, QProcess::ExitStatus exit_status);

private:
  void write_log(const QString data);

  void update_download();
  void init_install(const InstanceConfigureInstall* config);
  void start_install();

  void on_cancel();
  void on_failure(const std::stringstream& out);

private:
  const Instance& m_instance;

  TransferStatusListPtr m_download_status;
  QList<QProcess*> m_install_processes;

  QTimer* m_download_timer;

  QFile m_log_file;
  QTextStream m_log_file_stream;

  QFormLayout m_layout;
  QLabel* const m_label;
  QProgressBar* const m_progress_bar;
  CollapseButton* const m_collapse_button;
  QPlainTextEdit* const m_install_log_box;

  QDialogButtonBox* const m_buttons;
  QPushButton* const m_cancel_button;
  QPushButton* const m_retry_button;

private:
  InstanceInstallDialog(const InstanceInstallDialog&) = delete;
  InstanceInstallDialog& operator=(const InstanceInstallDialog&) = delete;
};
