/*
    SPDX-FileCopyrightText: 2004 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_SITES_H
#define KCDDB_SITES_H

#include <cddb.h>
#include <lookup.h>
#include <kcddb_export.h>
#include <QList>

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
