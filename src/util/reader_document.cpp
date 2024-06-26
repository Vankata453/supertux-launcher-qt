//  SuperTux
//  Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
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

#include "util/reader_document.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

#include <sexp/parser.hpp>

ReaderDocument
ReaderDocument::from_stream(std::istream& stream, const std::string& filename)
{
  sexp::Value sx = sexp::Parser::from_stream(stream, sexp::Parser::USE_ARRAYS);
  return ReaderDocument(filename, std::move(sx));
}

ReaderDocument
ReaderDocument::from_file(const std::string& filename)
{
  std::ifstream in;
  in.open(filename);
  if (!in.good())
  {
    std::stringstream msg;
    msg << "Parser problem: Couldn't open file '" << filename << "'.";
    throw std::runtime_error(msg.str());
  }
  else
  {
    ReaderDocument doc = from_stream(in, filename);
    in.close();
    return doc;
  }
}

ReaderDocument::ReaderDocument(const std::string& filename, sexp::Value sx) :
  m_filename(filename),
  m_sx(std::move(sx))
{
}

ReaderObject
ReaderDocument::get_root() const
{
  return ReaderObject(*this, m_sx);
}

std::string
ReaderDocument::get_filename() const
{
  return m_filename;
}

std::string
ReaderDocument::get_directory() const
{
  return std::filesystem::path(m_filename).parent_path();
}

/* EOF */
