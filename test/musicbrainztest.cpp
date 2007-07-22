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
#include "musicbrainztest.h"
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"


void MusicBrainzTest::testLookup()
{
  using namespace KCDDB;

  Client c;
  c.config().setCacheLookupEnabled(false);
  c.config().setFreedbLookupEnabled(false);
  c.config().setMusicBrainzLookupEnabled(true);

  TrackOffsetList list;

  //The Liptones / The Latest News
  list
    << 150
    << 9219
    << 20386
    << 34134
    << 51437
    << 68809
    << 86591
    << 106288
    << 115568
    << 133312
    << 155593
    << 169832
    << 179626
    << 194958
    << 212506;

  Result r = c.lookup(list);

  QVERIFY(r == Success);

  kDebug() << "Client::lookup gave : " << resultToString(r) << endl;

  CDInfoList response = c.lookupResponse();

  kDebug() << "Client::lookup returned : " << response.count() << " entries"
    << endl;

  QVERIFY(response.count() > 0);

  // See http://musicbrainz.org/showalbum.html?albumid=375639 for changes
  CDInfo i(response.first());
  QCOMPARE(i.numberOfTracks(),14);

  QCOMPARE(i.get(Artist).toString(),QString("The Liptones"));
  QCOMPARE(i.get(Title).toString(),QString("The Latest News"));
  // genre and year not really supported for musicbrainz
  QCOMPARE(i.get(Genre).toString(),QString());
  QCOMPARE(i.get(Year).toInt(),0);
  QCOMPARE(i.track(0).get(Title).toString(),QString("Jungle Heat"));
  QCOMPARE(i.track(1).get(Title).toString(),QString("It's All I Hear You Say"));
  QCOMPARE(i.track(2).get(Title).toString(),QString("Girl for Tonight"));
  QCOMPARE(i.track(3).get(Title).toString(),QString("Shoot Em Down"));
  QCOMPARE(i.track(4).get(Title).toString(),QString("Beautiful Day"));
  QCOMPARE(i.track(5).get(Title).toString(),QString("Paranoia"));
  QCOMPARE(i.track(6).get(Title).toString(),QString("My Way"));
  QCOMPARE(i.track(7).get(Title).toString(),QString("Dressed in Pink"));
  QCOMPARE(i.track(8).get(Title).toString(),QString("No Way Out"));
  QCOMPARE(i.track(9).get(Title).toString(),QString("Strange Kind of Justice"));
  QCOMPARE(i.track(10).get(Title).toString(),QString("Waiting Til Midnight"));
  QCOMPARE(i.track(11).get(Title).toString(),QString("Liptones"));
  QCOMPARE(i.track(12).get(Title).toString(),QString("Out With the Boys"));
  QCOMPARE(i.track(13).get(Title).toString(),QString("Free Like a Bird"));
  // comments not supported in a simple way
  for (int j=0; j < 14; j++)
    QCOMPARE(i.track(j).get(Comment).toString(),QString());

  // Make sure it's the same when loaded from the cache again
  c.config().setCacheLookupEnabled(true);
  c.config().setFreedbLookupEnabled(false);
  c.config().setMusicBrainzLookupEnabled(false);

  c.lookup(list);

  response = c.lookupResponse();
  QEXPECT_FAIL("", "Cache doesn't work yet", Continue);
  QVERIFY(response.count() > 0);
  // FIXME
  return;

  CDInfo cacheInfo(response.first());
  QCOMPARE(i.get(Artist).toString(),cacheInfo.get(Artist).toString());
  QCOMPARE(i.get(Title).toString(),cacheInfo.get(Title).toString());
  QCOMPARE(i.get(Genre).toString(),cacheInfo.get(Genre).toString());
  QCOMPARE(i.get(Year).toInt(),cacheInfo.get(Year).toInt());
  for (int j=0; j < 14; j++)
  {
    QCOMPARE(i.track(j).get(Title).toString(),cacheInfo.get(Title).toString());
    QCOMPARE(i.track(j).get(Comment).toString(),cacheInfo.get(Comment).toString());
  }
}

QTEST_KDEMAIN(MusicBrainzTest, NoGUI)

#include "musicbrainztest.moc"
