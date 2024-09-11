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

#include "dialog/instance_install_dialog.hpp"

#include <iostream>
#include <sstream>

#include <QDialogButtonBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>

#include "dialog/instance_configure_install.hpp"
#include "instance/instance.hpp"
#include "util/downloader.hpp"
#include "util/qt.hpp"
#include "version/version.hpp"
#include "widget/collapse_button.hpp"

InstanceInstallDialog::InstanceInstallDialog(QWidget* parent, const Instance& instance) :
  QDialog(parent),
  m_instance(instance),
  m_download_status(instance.request_download()),
  m_install_processes(),
  m_download_timer(new QTimer(this)),
  m_log_file(),
  m_log_file_stream(&m_log_file),
  m_layout(this),
  m_label(new QLabel(this)),
  m_progress_bar(new QProgressBar(this)),
  m_collapse_button(new CollapseButton(this, 1000, 600)),
  m_install_log_box(new QPlainTextEdit(this)),
  m_buttons(new QDialogButtonBox(this)),
  m_cancel_button(new QPushButton("Cancel", this)),
  m_retry_button(new QPushButton("Retry", this))
{
  setWindowTitle("Install Instance");
  setWindowModality(Qt::WindowModal);
  setAttribute(Qt::WA_DeleteOnClose);
  resize(500, height());

  m_layout.addWidget(m_label);
  m_layout.addWidget(m_progress_bar);
  m_layout.addWidget(m_buttons);

  m_buttons->addButton(m_retry_button, QDialogButtonBox::ResetRole);
  m_buttons->addButton(m_cancel_button, QDialogButtonBox::RejectRole);

  m_label->setAlignment(Qt::AlignHCenter);
  m_collapse_button->hide();
  m_collapse_button->set_text("Details");
  m_collapse_button->set_content(m_install_log_box);
  m_install_log_box->setReadOnly(true);

  QObject::connect(m_download_timer, &QTimer::timeout, this, &InstanceInstallDialog::update_download);

  QObject::connect(m_cancel_button, &QPushButton::released, this, &InstanceInstallDialog::on_cancel);
  QObject::connect(m_retry_button, &QPushButton::released, this, &InstanceInstallDialog::start_install);
  m_retry_button->hide();
}

void
InstanceInstallDialog::start()
{
  show();

  /** Stage 1: Download all required files for installing this instance. */
  m_label->setText("Downloading \"" + QString::fromStdString(m_instance.m_install_method->get_display_name()) +
                   "\" for version " + QString::fromStdString(m_instance.m_version->m_name) + "...");

  m_download_timer->start(0);
}

void
InstanceInstallDialog::update_download()
{
  if (m_download_status->is_active())
  {
    m_download_status->update();

    m_progress_bar->setValue(m_download_status->get_download_now());
    m_progress_bar->setMaximum(m_download_status->get_download_total());
  }
  else
  {
    m_download_timer->stop();

    if (!m_download_status->get_error().empty())
    {
      m_label->setText(QString::fromStdString(m_download_status->get_error()));
      m_cancel_button->setText("Close");
      return;
    }

    // If there are no install processes to run, the instance is now installed.
    if (!m_instance.has_install_processes())
    {
      std::stringstream out;
      out << "Instance \"" << m_instance.m_name << "\" was installed successfully!";
      std::cout << out.rdbuf() << std::endl;

      m_label->setText(QString::fromStdString(out.str()));
      m_cancel_button->setText("Close");
      return;
    }

    /** Stage 2: Show install setup dialog for this instance. */
    InstanceConfigureInstall* config = m_instance.create_configure_install_dialog();
    if (config)
    {
      QObject::connect(config, &QDialog::accepted,
        [this, config]()
        {
          init_install(config);
        });
      QObject::connect(config, &QDialog::rejected, this, &InstanceInstallDialog::on_cancel);

      config->show();
    }
    else
    {
      init_install(nullptr);
    }
  }
}

void
InstanceInstallDialog::init_install(const InstanceConfigureInstall* config)
{
  /** Stage 3: Set up install processes, start the install. */
  m_layout.insertRow(2, m_collapse_button);
  m_layout.insertRow(3, m_install_log_box);
  m_collapse_button->show();

  m_install_processes = m_instance.create_install_processes(config);
  for (auto it = m_install_processes.begin(); it != m_install_processes.end(); ++it)
  {
    QProcess* process = *it;
    process->setParent(this);

    QObject::connect(process, &QProcess::readyReadStandardOutput,
      [this, process]()
      {
        write_log(process->readAllStandardOutput());
      });
    QObject::connect(process, &QProcess::readyReadStandardError,
      [this, process]()
      {
        write_log(process->readAllStandardError());
      });

    QObject::connect(process, &QProcess::errorOccurred,
      [this, process](QProcess::ProcessError err)
      {
        on_install_process_error(process, err);
      });
    QObject::connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
      [this, it](int exit_code, QProcess::ExitStatus exit_status)
      {
        on_install_process_finish(it, exit_code, exit_status);
      });
  }

  start_install();
}

void
InstanceInstallDialog::start_install()
{
  m_label->setText("Installing \"" + QString::fromStdString(m_instance.m_install_method->get_display_name()) + "\"...\n"
                   "Process 1/" + QString::number(m_install_processes.size()));

  m_retry_button->hide();
  m_cancel_button->hide();
  m_progress_bar->setMinimum(0);
  m_progress_bar->setMaximum(0);
  m_progress_bar->setValue(0);
  m_install_log_box->setPlainText(QString());

  // Open a new process log file
  m_log_file.setFileName(m_instance.get_build_log_filename());
  m_log_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

  // Run the first process, which on successful finish will run the next one and etc...
  m_install_processes.first()->start();

  // Log first process run
  write_log("> " + util::qt::get_command_process_arguments(m_install_processes.first()).join(" ") + "\n\n");
}

void
InstanceInstallDialog::on_install_process_error(const QProcess* process, QProcess::ProcessError err)
{
  std::stringstream out;
  out << "Error running install process for instance \"" << m_instance.m_id << "\": " << util::qt::process_error_to_string(err) << std::endl
      << "Command: " << util::qt::get_command_process_arguments(process).join(" ").toStdString();
  on_failure(out);
}

void
InstanceInstallDialog::on_install_process_finish(QList<QProcess*>::iterator it, int exit_code, QProcess::ExitStatus exit_status)
{
  if (exit_code != 0)
  {
    std::stringstream out;
    out << "Install process for instance \"" << m_instance.m_id << "\" didn't finish successfully!" << std::endl
        << "Command: " << util::qt::get_command_process_arguments(*it).join(" ").toStdString() << std::endl
        << "Exit code: " << exit_code;
    on_failure(out);
  }
  else if (exit_status == QProcess::CrashExit)
  {
    std::stringstream out;
    out << "Install process for instance \"" << m_instance.m_id << "\" has quit unexpectedly!" << std::endl
        << "Command: " << util::qt::get_command_process_arguments(*it).join(" ").toStdString() << std::endl
        << "Exit code: " << exit_code;
    on_failure(out);
  }
  else
  {
    if (++it == m_install_processes.end())
    {
      std::stringstream out;
      out << "Instance \"" << m_instance.m_name << "\" was installed successfully!";
      std::cout << out.rdbuf() << std::endl;

      m_label->setText(QString::fromStdString(out.str()));
      m_progress_bar->setMaximum(1);
      m_progress_bar->setValue(1);
      m_cancel_button->show();
      m_cancel_button->setText("Close");
    }
    else
    {
      // Log next process run
      write_log("\n> " + util::qt::get_command_process_arguments(*it).join(" ") + "\n\n");

      (*it)->start(); // Run next install process
    }
  }
}

void
InstanceInstallDialog::write_log(const QString data)
{
  // Append to log file
  m_log_file_stream << data;

  // Append to log box
  m_install_log_box->moveCursor(QTextCursor::End);
  m_install_log_box->insertPlainText(data);
  m_install_log_box->moveCursor(QTextCursor::End);
}

void
InstanceInstallDialog::on_cancel()
{
  if (m_download_status->is_active())
    m_download_status->abort();

  close();
}

void
InstanceInstallDialog::on_failure(const std::stringstream& out)
{
  std::cout << out.rdbuf() << std::endl;

  m_log_file.close();

  m_label->setText(QString::fromStdString(out.str()));
  m_progress_bar->setMaximum(1);
  m_collapse_button->set_checked(true);
  m_retry_button->show();
  m_cancel_button->show();
  m_cancel_button->setText("Close");
}
