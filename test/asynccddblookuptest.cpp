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

#include <qtest_kde.h>
#include "asynccddblookuptest.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"

void AsyncCDDBLookupTest::testLookup()
{
  using namespace KCDDB;

  client_ = new Client;
  client_->config().setHostname("freedb.freedb.org");
  client_->config().setPort(8880);
  client_->config().setCachePolicy(Cache::Ignore);
  client_->config().setLookupTransport(Lookup::CDDBP);
  client_->setBlockingMode( false );

  connect(client_,SIGNAL(finished(KCDDB::Result)),SLOT(slotFinished(KCDDB::Result)));

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
  // See: http://www.freedb.org/freedb/jazz/a1107d0a for updated data
  QCOMPARE(m_info.get("revision").toInt(), 4);

  QCOMPARE(m_info.get(Artist).toString(),QString("Kruder & Dorfmeister"));
  QCOMPARE(m_info.get(Title).toString(),QString("K&D Sessions"));
  QCOMPARE(m_info.get(Genre).toString(),QString("Tribal"));
  QCOMPARE(m_info.get(Year).toInt(),1998);
  QCOMPARE(m_info.track(0).get(Title).toString(),QString("Heroes [kruder's long loose bossa]"));
  QCOMPARE(m_info.track(1).get(Title).toString(),QString("Jazz Master [k&d session]"));
  QCOMPARE(m_info.track(2).get(Title).toString(),QString("Speechless [drum 'n' bass]"));
  QCOMPARE(m_info.track(3).get(Title).toString(),QString("Going Under [main version - k&d session]"));
  QCOMPARE(m_info.track(4).get(Title).toString(),QString("Bug Powder Dust [k&d session]"));
  QCOMPARE(m_info.track(5).get(Title).toString(),QString("Rollin' On Chrome [wild motherfucker dub]"));
  QCOMPARE(m_info.track(6).get(Title).toString(),QString("Useless [k&d session]"));
  QCOMPARE(m_info.track(7).get(Title).toString(),QString("Gotta Jazz [dorfmeister remix]"));
  QCOMPARE(m_info.track(8).get(Title).toString(),QString::fromUtf8("Donaueschingen [peter kruder's donaudampfschifffahrtsgesellschaftskapitänskajütenremix]"));
  QCOMPARE(m_info.track(9).get(Title).toString(),QString("Trans Fatty Acid [k&d session]"));
  QCOMPARE(m_info.track(0).get(Artist).toString(),QString("Roni Size"));
  QCOMPARE(m_info.track(1).get(Artist).toString(),QString("Alex Reece"));
  QCOMPARE(m_info.track(2).get(Artist).toString(),QString("Count Basic"));
  QCOMPARE(m_info.track(3).get(Artist).toString(),QString("Rocker's HiFi"));
  QCOMPARE(m_info.track(4).get(Artist).toString(),QString("Bomb The Bass"));
  QCOMPARE(m_info.track(5).get(Artist).toString(),QString("Aphrodelics"));
  QCOMPARE(m_info.track(6).get(Artist).toString(),QString("Depeche Mode"));
  QCOMPARE(m_info.track(7).get(Artist).toString(),QString("Count Basic"));
  QCOMPARE(m_info.track(8).get(Artist).toString(),QString::fromUtf8("Trüby Trio"));
  QCOMPARE(m_info.track(9).get(Artist).toString(),QString("Lamb"));
}

  void
AsyncCDDBLookupTest::slotFinished(Result r)
{
  m_result = r;
  kDebug() << "AsyncCDDBLookupTest::slotResult: Got " << KCDDB::resultToString(r) << endl;

  CDInfoList l = client_->lookupResponse();

  kDebug() << "AsyncCDDBLookupTest::slotResult: Item count: " <<  l.count() << endl;

  foreach(CDInfo i, l)
  {
    if (i.get("discid") == "a1107d0a" && i.get(Category) == "jazz")
    {
      kDebug() << "Found the CD" << endl;
      m_info = i;
      break;
    }
  }

  m_eventLoop.quit();
}

QTEST_KDEMAIN(AsyncCDDBLookupTest, NoGUI)

#include "asynccddblookuptest.moc"
