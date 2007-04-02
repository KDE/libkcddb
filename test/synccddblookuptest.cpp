/*
  Copyright (C) 2005 Richard Lärkäng <nouseforaname@home.se>

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
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include "synccddblookuptest.h"

void SyncCDDBLookupTest::testLookup()
{
  using namespace KCDDB;

  Client c;
  c.config().setHostname("freedb.freedb.org");
  c.config().setPort(8880);
  c.config().setCachePolicy(Cache::Ignore);
  c.config().setLookupTransport(Lookup::CDDBP);

  TrackOffsetList list;

  list
    << 150
    << 106965
    << 127220
    << 151925
    << 176085
    << 234500;

  kDebug()
    << CDDB::trackOffsetListToId(list)
    << " "
    //<< trackOffsetListToString(list)
    << endl;

  c.lookup(list);

  CDInfoList response = c.lookupResponse();

  CDInfoList::ConstIterator it;

  bool hasRunTest = false;

  for (it = response.begin(); it != response.end(); ++it)
  {
    CDInfo i(*it);

    if (i.get("discid") == "3e0c3a05" && i.get(Category) == "misc")
    {
      // If revision doesn't match, test probably needs to be updated
      // See: http://www.freedb.org/freedb/misc/3e0c3a05 for updated data
      QCOMPARE(i.get("revision").toInt(), 16);

      QCOMPARE(i.get(Artist).toString(),QString("Pink Floyd"));
      QCOMPARE(i.get(Title).toString(),QString("Atom Heart Mother"));
      QCOMPARE(i.get(Genre).toString(),QString("Psychedelic Rock"));
      QCOMPARE(i.get(Year).toInt(),1970);
      QCOMPARE(i.track(0).get(Title).toString(),QString("Atom heart mother: a) Father's shout b) Breast milky c) Mother fore d) Funky dung e) Mind your throats please f) Remergence"));
      QCOMPARE(i.track(1).get(Title).toString(),QString("If"));
      QCOMPARE(i.track(2).get(Title).toString(),QString("Summer '68"));
      QCOMPARE(i.track(3).get(Title).toString(),QString("Fat Old Sun"));
      QCOMPARE(i.track(4).get(Title).toString(),QString("Alan's psychedelic breakfast: a) Rise and shine b) Sunny side up c) Morning glory"));
      QCOMPARE(i.track(0).get(Comment).toString(),QString(""));
      QCOMPARE(i.track(1).get(Comment).toString(),QString(""));
      QCOMPARE(i.track(2).get(Comment).toString(),QString(""));
      QCOMPARE(i.track(3).get(Comment).toString(),QString(""));
      QCOMPARE(i.track(4).get(Comment).toString(),QString(""));
      for (int j=0; j <= 4; j++)
      {
        QCOMPARE(i.track(j).get(Artist).toString(),QString("Pink Floyd"));
      }

      hasRunTest = true;
    }
  }

  QVERIFY(hasRunTest);
}

QTEST_KDEMAIN(SyncCDDBLookupTest, NoGUI)

#include "synccddblookuptest.moc"
