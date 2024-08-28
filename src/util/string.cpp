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

namespace util {

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

// From https://github.com/SuperTux/supertux/blob/master/src/util/file_system.cpp
std::string file_basename(const std::string& filename)
{
  std::string::size_type p = filename.find_last_of('/');
  if (p == std::string::npos)
    p = filename.find_last_of('\\');
  if (p == std::string::npos)
    return filename;

  return filename.substr(p+1, filename.size()-p-1);
}

// From https://github.com/SuperTux/supertux/blob/master/src/util/file_system.cpp
std::string file_join(const std::string& lhs, const std::string& rhs)
{
  if (lhs.empty())
  {
    return rhs;
  }
  else if (rhs.empty())
  {
    return lhs + "/";
  }
  else if (lhs.back() == '/' && rhs.front() != '/')
  {
    return lhs + rhs;
  }
  else if (lhs.back() != '/' && rhs.front() == '/')
  {
    return lhs + rhs;
  }
  else if (lhs.back() == '/' && rhs.front() == '/')
  {
    return lhs + rhs.substr(1);
  }
  else
  {
    return lhs + "/" + rhs;
  }
}

} // namespace util
