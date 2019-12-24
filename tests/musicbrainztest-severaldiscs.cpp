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

#include "musicbrainztest-severaldiscs.h"
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include "config-musicbrainz.h"
#include <QTest>

void MusicBrainzTestSeveralDiscs::testLookup()
{
  using namespace KCDDB;

#ifndef HAVE_MUSICBRAINZ5
  QSKIP("This test requires libmusicbrainz", SkipAll);
#endif

  Client c;
  c.config().setCacheLookupEnabled(false);
  c.config().setFreedbLookupEnabled(false);
  c.config().setMusicBrainzLookupEnabled(true);

  TrackOffsetList list;

  list <<
    150 <<
    17272 <<
    36395 <<
    47954 <<
    63638 <<
    77928 <<
    95029 <<
    112094 <<
    130540 <<
    145938 <<
    160463 <<
    180364 <<
    195549 <<
    203821 <<
    222410 <<
    230367 <<
    249241 <<
    273944 <<
    287584 <<
    309904 <<
    325124;

  Result r = c.lookup(list);

  QVERIFY(r == Success);

  qDebug() << "Client::lookup gave : " << resultToString(r);

  CDInfoList response = c.lookupResponse();

  qDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  QVERIFY(response.count() > 0);

  // See http://musicbrainz.org/release/4e0655b5-22fc-496e-9c14-2ee1a6027b45 for changes (disc 2)
  CDInfo i(response.first());
  QCOMPARE(i.numberOfTracks(),20);

  QCOMPARE(i.get(Artist).toString(),QString::fromUtf8("Billy Bragg"));
  QCOMPARE(i.get(Title).toString(),QString::fromUtf8("Must I Paint You a Picture? The Essential Billy Bragg (disc 2)"));
  // genre not really supported for musicbrainz
  QCOMPARE(i.get(Genre).toString(),QString());
  QCOMPARE(i.get(Year).toInt(),2003);
  QCOMPARE(i.track(0).get(Title).toString(),QString::fromUtf8("Sexuality"));
  QCOMPARE(i.track(1).get(Title).toString(),QString::fromUtf8("Cindy of 1000 Lives"));
  QCOMPARE(i.track(2).get(Title).toString(),QString::fromUtf8("Moving the Goalposts"));
  QCOMPARE(i.track(3).get(Title).toString(),QString::fromUtf8("Tank Park Salute"));
  QCOMPARE(i.track(4).get(Title).toString(),QString::fromUtf8("You Woke Up My Neighbourhood"));
  QCOMPARE(i.track(5).get(Title).toString(),QString::fromUtf8("Accident Waiting to Happen (Red Stars version)"));
  QCOMPARE(i.track(6).get(Title).toString(),QString::fromUtf8("Sulk"));
  QCOMPARE(i.track(7).get(Title).toString(),QString::fromUtf8("Upfield"));
  QCOMPARE(i.track(8).get(Title).toString(),QString::fromUtf8("The Fourteenth of February"));
  QCOMPARE(i.track(9).get(Title).toString(),QString::fromUtf8("Brickbat"));
  QCOMPARE(i.track(10).get(Title).toString(),QString::fromUtf8("The Space Race Is Over"));
  QCOMPARE(i.track(11).get(Title).toString(),QString::fromUtf8("The Boy Done Good"));
  QCOMPARE(i.track(12).get(Title).toString(),QString::fromUtf8("Ingrid Bergman"));
  QCOMPARE(i.track(13).get(Title).toString(),QString::fromUtf8("Way Over Yonder in the Minor Key"));
  QCOMPARE(i.track(14).get(Title).toString(),QString::fromUtf8("My Flying Saucer"));
  QCOMPARE(i.track(15).get(Title).toString(),QString::fromUtf8("All You Fascists Bound to Lose (Blokes version)"));
  QCOMPARE(i.track(16).get(Title).toString(),QString::fromUtf8("NPWA"));
  QCOMPARE(i.track(17).get(Title).toString(),QString::fromUtf8("St. Monday"));
  QCOMPARE(i.track(18).get(Title).toString(),QString::fromUtf8("Somedays I See the Point"));
  QCOMPARE(i.track(19).get(Title).toString(),QString::fromUtf8("Take Down the Union Jack (Band version)"));
  for (int j=0; j < 12; j++)
    QCOMPARE(i.track(j).get(Artist).toString(),QString::fromUtf8("Billy Bragg"));
  for (int j=12; j < 15; j++)
    QCOMPARE(i.track(j).get(Artist).toString(),QString::fromUtf8("Billy Bragg & Wilco"));
  for (int j=15; j < 20; j++)
    QCOMPARE(i.track(j).get(Artist).toString(),QString::fromUtf8("Billy Bragg and The Blokes"));
  // comments not supported in a simple way
  for (int j=0; j < 19; j++)
    QCOMPARE(i.track(j).get(Comment).toString(),QString());

  // Make sure it's the same when loaded from the cache again
  c.config().setCacheLookupEnabled(true);
  c.config().setFreedbLookupEnabled(false);
  c.config().setMusicBrainzLookupEnabled(false);

  c.lookup(list);

  response = c.lookupResponse();
  QVERIFY(response.count() > 0);

  CDInfo cacheInfo(response.first());
  QCOMPARE(i.get(Artist).toString(),cacheInfo.get(Artist).toString());
  QCOMPARE(i.get(Title).toString(),cacheInfo.get(Title).toString());
  QCOMPARE(i.get(Year).toInt(),cacheInfo.get(Year).toInt());
  for (int j=0; j < 19; j++)
  {
    QCOMPARE(i.track(j).get(Title).toString(),cacheInfo.track(j).get(Title).toString());
    QCOMPARE(i.track(j).get(Comment).toString(),cacheInfo.track(j).get(Comment).toString());
  }
}

QTEST_GUILESS_MAIN(MusicBrainzTestSeveralDiscs)
