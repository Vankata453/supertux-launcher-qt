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

#include "version/version.hpp"

#include <iostream>
#include <optional>

#include "instance/instance.hpp"
#include "util/platform.hpp"
#include "util/reader_document.hpp"
#include "util/reader_mapping.hpp"

Version::Version(const std::string& file) :
  m_name(),
  m_install_methods(),
  m_support_userdir(),
  m_support_developer_mode()
{
  auto doc = ReaderDocument::from_file(file);
  auto root = doc.get_root();
  if (root.get_name() != "supertux-launcher-version")
    throw std::runtime_error("File is not a \"supertux-launcher-version\" file!");

  auto mapping = root.get_mapping();

  mapping.get("name", m_name);
  if (m_name.empty())
    throw std::runtime_error("No version name specified!");

  std::optional<ReaderMapping> support_mapping;
  if (mapping.get("support", support_mapping))
  {
    support_mapping->get("userdir", m_support_userdir);
    support_mapping->get("developer-mode", m_support_developer_mode);
  }

  std::optional<ReaderMapping> install_methods_mapping;
  if (mapping.get("install-methods", install_methods_mapping))
  {
    auto install_methods_iter = install_methods_mapping->get_iter();
    while (install_methods_iter.next())
    {
      try
      {
        m_install_methods.emplace(InstallMethod::from_string(install_methods_iter.get_key())->get_type(),
                                  install_methods_iter.as_mapping());
      }
      catch (const std::exception& err)
      {
        std::cout << "Couldn't load install method \"" << install_methods_iter.get_key()
                  << "\" for version \"" << m_name << "\": " << err.what() << std::endl;
      }
    }
  }
}

QStringList
Version::get_run_arguments(const Instance& instance) const
{
  QStringList args;

  if (m_support_userdir)
    args << "--userdir" << instance.get_data_directory().canonicalPath();

  return args;
}
