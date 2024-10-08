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

#include "util/qt.hpp"

#include "util/platform.hpp"

namespace util {

namespace qt {

QProcess* command_to_process(const QString& command)
{
  QProcess* process = new QProcess;

  process->setProgram(
#ifdef PLATFORM_WIN
    "cmd"
#elifdef PLATFORM_LINUX
    "sh"
#endif
  );
  process->setArguments(
    QStringList()
#ifdef PLATFORM_WIN
      << "/c"
#elifdef PLATFORM_LINUX
      << "-c"
#endif
      << command
  );

  return process;
}

QStringList get_command_process_arguments(const QProcess* process)
{
  QStringList args = process->arguments();
  args.removeFirst(); // First argument is the command specifier for the command line.
  return args;
}

std::string process_error_to_string(QProcess::ProcessError err)
{
  switch (err)
  {
    case QProcess::FailedToStart:
      return "Process failed to start!";
    case QProcess::Crashed:
      return "Process crashed!";
    case QProcess::Timedout:
      return "Process has timed out!";
    case QProcess::WriteError:
      return "Error writing to process!";
    case QProcess::ReadError:
      return "Error reading from process!";
    default:
      return "Unknown error!";
  }
}

} // namespace qt

} // namespace util
