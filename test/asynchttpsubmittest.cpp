/*
  Copyright (C) 2006 Richard Lärkäng <nouseforaname@home.se>

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

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

#include "asynchttpsubmittest.h"
#include "libkcddb/submit.h"

AsyncHTTPSubmitTest::AsyncHTTPSubmitTest(QCoreApplication& app)
  : app_(app)
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setFreedbSubmitTransport(Submit::HTTP);
  client_->setBlockingMode( false );

  TrackOffsetList list;

  list
    << 150      // First track start.
    << 2592
    << 35472
    << 47891
    << 123310
    << 133125;  // Disc end.

  CDInfo cdInfo;

  cdInfo.set("discid", "3606ed05");
  cdInfo.set("revision", 4);
  cdInfo.set("title", "Bamse och Bronto");
  cdInfo.set("artist", "Musiksage");
  cdInfo.set("year", 2001);
  cdInfo.set("category", "misc");
  cdInfo.set("genre", "Barnsaga");
  cdInfo.set("extd", QString::fromUtf8("Berättare: Olof Thunberg"));

  cdInfo.track(0).set("title", "Bamses signaturmelodi");
  cdInfo.track(1).set("title","*");
  cdInfo.track(2).set("title", "Brummavisan");
  cdInfo.track(3).set("title", "*");
  cdInfo.track(4).set("title", QString::fromUtf8("Jätteödlan Bronto"));

  connect(client_,SIGNAL(finished(KCDDB::Result)),SLOT(slotFinished(KCDDB::Result)));

  client_->submit(cdInfo, list);
}

  void
AsyncHTTPSubmitTest::slotFinished(Result r)
{
  qDebug() << "AsyncHTTPSubmitTest::slotFinished: Got " << KCDDB::resultToString(r);

  app_.quit();
}

int main(int argc, char ** argv)
{
  QCoreApplication app(argc, argv);
  app.setApplicationName("libkcddb_test");

  new AsyncHTTPSubmitTest(app);

  return app.exec();
}
