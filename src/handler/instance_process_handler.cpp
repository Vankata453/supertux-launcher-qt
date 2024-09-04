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

#include "handler/instance_process_handler.hpp"

#include <iostream>
#include <sstream>

#include <QMessageBox>

#include "instance/instance.hpp"
#include "util/qt.hpp"
#include "version/version.hpp"
#include "window/main.hpp"

void
InstanceProcessHandler::start_process(Instance& instance)
{
  if (instance.is_running())
  {
    std::stringstream out;
    out << "Couldn't launch instance \"" << instance.m_id << "\": Instance is already running!";
    std::cout << out.rdbuf() << std::endl;

    QMessageBox::critical(MainWindow::current(), "Error launching instance!", QString::fromStdString(out.str()));
    return;
  }

  try
  {
    QProcess* process = instance.create_process();

    const QString log_file = instance.get_run_log_filename();
    process->setStandardOutputFile(log_file, QIODevice::Append);
    process->setStandardErrorFile(log_file, QIODevice::Append);

    QObject::connect(process, &QProcess::errorOccurred,
      [&instance](QProcess::ProcessError err)
      {
        on_error(instance, err);
      });
    QObject::connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
      [&instance](int exit_code, QProcess::ExitStatus exit_status)
      {
        on_finish(instance, exit_code, exit_status);
      });

    std::cout << std::endl
              << "========LAUNCHING INSTANCE \"" << instance.m_name << "\" ("
              << instance.m_version->m_name << ", " << instance.m_install_method->get_display_name()
              << ")========"
              << std::endl
              << std::endl;
    process->start();

    instance.m_running = true;
  }
  catch (const std::exception& err)
  {
    instance.m_running = false;

    std::stringstream out;
    out << "Couldn't launch instance \"" << instance.m_id << "\": " << err.what();
    std::cout << out.rdbuf() << std::endl;

    QMessageBox::critical(MainWindow::current(), "Error launching instance!", QString::fromStdString(out.str()));
  }
}

void
InstanceProcessHandler::on_error(Instance& instance, QProcess::ProcessError err)
{
  instance.m_running = false;

  if (err == QProcess::Crashed)
    return; // Will be handled by on_finish().

  std::stringstream out;
  out << "Couldn't launch instance \"" << instance.m_id << "\": " << util::qt::process_error_to_string(err) << std::endl;
  std::cout << out.rdbuf() << std::endl;

  QMessageBox::critical(MainWindow::current(), "Error launching instance!", QString::fromStdString(out.str()));
}

void
InstanceProcessHandler::on_finish(Instance& instance, int exit_code, QProcess::ExitStatus exit_status)
{
  instance.m_running = false;

  if (exit_code != 0)
  {
    std::stringstream out;
    out << "Instance \"" << instance.m_name << "\" has not quit successfully!";
    std::cout << std::endl
              << out.rdbuf();

    QMessageBox::critical(MainWindow::current(), "Instance: Unsuccessful exit", QString::fromStdString(out.str()));
    MainWindow::current()->show_instance_options(instance);
  }
  else if (exit_status == QProcess::CrashExit)
  {
    std::stringstream out;
    out << "Instance \"" << instance.m_name << "\" has quit unexpectedly!";
    std::cout << std::endl
              << out.rdbuf();

    QMessageBox::critical(MainWindow::current(), "Instance: Unexpected exit", QString::fromStdString(out.str()));
    MainWindow::current()->show_instance_options(instance);
  }
  else
  {
    std::cout << std::endl
          << "Instance \"" << instance.m_name << "\" has quit normally.";
  }
  std::cout << " Exit code: " << exit_code << std::endl;
}
