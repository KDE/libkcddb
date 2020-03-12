/*
  Copyright (C) 2003 Richard Lärkäng <nouseforaname@home.se>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include <QDebug>
#include <QCoreApplication>

#include "libkcddb/client.h"
#include "libkcddb/kcddbconfig.h"
#include "libkcddb/submit.h"

  int
main(int argc, char ** argv)
{
  QCoreApplication app(argc, argv);
  app.setApplicationName(QString::fromUtf8("libkcddb_test"));

  using namespace KCDDB;

  TrackOffsetList list;

  list
    << 150      // First track start.
    << 2592
    << 35472
    << 47891
    << 123310
    << 133125;  // Disc end.

  CDInfo cdInfo;

  cdInfo.set(QString::fromUtf8("discid"), QString::fromUtf8("3606ed05"));
  cdInfo.set(QString::fromUtf8("revision"), 4);
  cdInfo.set(QString::fromUtf8("title"), QString::fromUtf8("Bamse och Bronto"));
  cdInfo.set(QString::fromUtf8("artist"), QString::fromUtf8("Musiksage"));
  cdInfo.set(QString::fromUtf8("year"), 2001);
  cdInfo.set(QString::fromUtf8("category"), QString::fromUtf8("misc"));
  cdInfo.set(QString::fromUtf8("genre"), QString::fromUtf8("Barnsaga"));
  cdInfo.set(QString::fromUtf8("extd"), QString::fromUtf8("Berättare: Olof Thunberg"));

  cdInfo.track(0).set(QString::fromUtf8("title"), QString::fromUtf8("Bamses signaturmelodi"));
  cdInfo.track(1).set(QString::fromUtf8("title"),QString::fromUtf8("*"));
  cdInfo.track(2).set(QString::fromUtf8("title"), QString::fromUtf8("Brummavisan"));
  cdInfo.track(3).set(QString::fromUtf8("title"), QString::fromUtf8("*"));
  cdInfo.track(4).set(QString::fromUtf8("title"), QString::fromUtf8("Jätteödlan Bronto"));

  Client c;
  c.config().setFreedbSubmitTransport(Submit::SMTP);
  c.config().setSubmitAddress(QString::fromUtf8("test-submit@gnudb.org"));

  c.setBlockingMode( true );

  Result r = c.submit(cdInfo, list);

  qDebug() << "Result: " << resultToString(r);
}

