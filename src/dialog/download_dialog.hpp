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

#include <QProgressDialog>

#include "util/downloader_defines.hpp"

class DownloadDialog final : public QProgressDialog
{
public:
  DownloadDialog(const std::string& title, const std::string& success_title,
                 TransferStatusPtr status);
  DownloadDialog(const std::string& title, const std::string& success_title,
                 TransferStatusListPtr status);

  void start();

private:
  TransferStatusListPtr m_status;
  const std::string m_success_title;

private:
  DownloadDialog(const DownloadDialog&) = delete;
  DownloadDialog& operator=(const DownloadDialog&) = delete;
};
