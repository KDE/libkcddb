/*
  Copyright ( C ) 2004 Richard Lärkäng <nouseforaname@home.se>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (  at your option ) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef KCDDB_SITES_H
#define KCDDB_SITES_H

#include <QtCore/QList>
#include <libkcddb/cddb.h>
#include <libkcddb/lookup.h>
#include <libkcddb/kcddb_export.h>

namespace KCDDB
{
  class Mirror
  {
    public:
      QString address;
      Lookup::Transport transport;
      uint port;
      QString description;
  } ;

  class KCDDB_EXPORT Sites
  {
    public:
      Sites();

      QList<Mirror> siteList();
    private:
      QList<Mirror> readData(const QByteArray& data);
      Mirror parseLine(const QString& line);
  } ;
}

#endif

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
