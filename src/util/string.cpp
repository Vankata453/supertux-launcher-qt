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

#include "util/string.hpp"

#include <iomanip>
#include <sstream>

namespace util {

std::string timestamp_to_date_string(std::time_t timestamp, const char* format)
{
  std::stringstream out;
  out << std::put_time(std::localtime(&timestamp), format);
  return out.str();
}

QStringList to_qt_string_list(const std::vector<std::string>& vector)
{
  QStringList list;

  list.reserve(vector.size());
  std::transform(vector.begin(), vector.end(), std::back_inserter(list),
                 [](const std::string& str)
                 {
                   return QString::fromStdString(str);
                 });

  return list;
}

} // namespace util
