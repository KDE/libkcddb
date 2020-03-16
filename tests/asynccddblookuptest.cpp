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

#include "asynccddblookuptest.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include <QTest>

void AsyncCDDBLookupTest::testLookup()
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setHostname(QString::fromUtf8("gnudb.gnudb.org"));
  client_->config().setPort(8880);
  client_->config().setCacheLookupEnabled(false);
  client_->config().setFreedbLookupEnabled(true);
  client_->config().setMusicBrainzLookupEnabled(false);
  client_->config().setFreedbLookupTransport(Lookup::CDDBP);
  client_->setBlockingMode( false );

  connect(client_,&KCDDB::Client::finished,this, &AsyncCDDBLookupTest::slotFinished);

  TrackOffsetList list;

  // a1107d0a - Kruder & Dorfmeister - The K&D Sessions - Disc One.
  list
    << 150      // First track start.
    << 29462
    << 66983
    << 96785
    << 135628
    << 168676
    << 194147
    << 222158
    << 247076
    << 278203   // Last track start.
    << 316732;  // Disc end.

  client_->lookup(list);

  m_eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

  delete client_;

  QVERIFY(m_result == Success);

  // If revision doesn't match, test probably needs to be updated
  // See: http://www.gnudb.org/gnudb/jazz/a1107d0a for updated data
  QCOMPARE(m_info.get(QString::fromUtf8("revision")).toInt(), 7);

  QCOMPARE(m_info.get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.get(Title).toString(),QString::fromUtf8("The K&D Sessions (Part One)"));
  QCOMPARE(m_info.get(Genre).toString(),QString::fromUtf8("Trip-Hop"));
  QCOMPARE(m_info.get(Year).toInt(),1998);
  QCOMPARE(m_info.track(0).get(Title).toString(),QString::fromUtf8("Roni Size - Heroes (Kruder's Long Loose Bossa remixed by Peter Kruder)"));
  QCOMPARE(m_info.track(1).get(Title).toString(),QString::fromUtf8("Alex Reece - Jazz Master (K&D Session)"));
  QCOMPARE(m_info.track(2).get(Title).toString(),QString::fromUtf8("Count Basic - Speechless (Drum 'n' Bass remixed by Peter Kruder)"));
  QCOMPARE(m_info.track(3).get(Title).toString(),QString::fromUtf8("Rockers Hi-Fi - Going Under (K&D Session)"));
  QCOMPARE(m_info.track(4).get(Title).toString(),QString::fromUtf8("Bomb The Bass - Bug Powder Dust (K&D Session)"));
  QCOMPARE(m_info.track(5).get(Title).toString(),QString::fromUtf8("Aphrodelics - Rollin' On Chrome (Wild Motherfucker Dub remixed by Richard Dorfmeister)"));
  QCOMPARE(m_info.track(6).get(Title).toString(),QString::fromUtf8("Depeche Mode - Useless (K&D Session)"));
  QCOMPARE(m_info.track(7).get(Title).toString(),QString::fromUtf8("Count Basic - Gotta Jazz (remixed by Richard Dorfmeister)"));
  QCOMPARE(m_info.track(8).get(Title).toString(),QString::fromUtf8("Rainer Truby Trio - Donaueschingen (Peter Kruder's Donaudampfschifffahrtsgesellschaftskapitnskajtenremix)"));
  QCOMPARE(m_info.track(9).get(Title).toString(),QString::fromUtf8("Lamb - Trans Fatty Acid (K&D Session)"));
  QCOMPARE(m_info.track(0).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(1).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(2).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(3).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(4).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(5).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(6).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(7).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(8).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
  QCOMPARE(m_info.track(9).get(Artist).toString(),QString::fromUtf8("Kruder & Dorfmeister"));
}

  void
AsyncCDDBLookupTest::slotFinished(Result r)
{
  m_result = r;
  qDebug() << "AsyncCDDBLookupTest::slotResult: Got " << KCDDB::resultToString(r);

  CDInfoList l = client_->lookupResponse();

  qDebug() << "AsyncCDDBLookupTest::slotResult: Item count: " <<  l.count();

  foreach(const CDInfo &i, l)
  {
    if (i.get(QString::fromUtf8("discid")) == QString::fromUtf8("a1107d0a") && i.get(Category) == QString::fromUtf8("jazz"))
    {
      qDebug() << "Found the CD";
      m_info = i;
      break;
    }
  }

  m_eventLoop.quit();
}

QTEST_GUILESS_MAIN(AsyncCDDBLookupTest)
