/*
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QDebug>
#include <QCoreApplication>
#include <QList>

#include "libkcddb/sites.h"

  int
main(int argc, char ** argv)
{
  QCoreApplication app(argc, argv);
  app.setApplicationName(QString::fromUtf8("libkcddb_test"));

  using namespace KCDDB;

  Sites s;

  qDebug() << "Sites: ";

  QList<Mirror> sites = s.siteList();
  for (QList<Mirror>::Iterator it = sites.begin(); it != sites.end(); ++it)
    if ((*it).transport == Lookup::CDDBP)
      qDebug() << (*it).address << " CDDBP " << (*it).port << " " << (*it).description;
    else
      qDebug() << (*it).address << " HTTP " << (*it).port << " " << (*it).description;

  return 0;
}
