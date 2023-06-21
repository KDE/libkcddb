/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asynchttpsubmittest.h"
#include <QDebug>
#include <QCoreApplication>

#include "libkcddb/submit.h"

AsyncHTTPSubmitTest::AsyncHTTPSubmitTest(QCoreApplication& app)
  : app_(app)
{
  using namespace KCDDB;

  client_ = new Client;
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

  connect(client_,&KCDDB::Client::finished,this, &AsyncHTTPSubmitTest::slotFinished);

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
  app.setApplicationName(QString::fromUtf8("libkcddb_test"));

  new AsyncHTTPSubmitTest(app);

  return app.exec();
}

#include "moc_asynchttpsubmittest.cpp"
