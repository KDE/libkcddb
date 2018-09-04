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

#include "synccddblookuptest.h"
#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include <QTest>

void SyncCDDBLookupTest::testLookup()
{
  using namespace KCDDB;

  Client c;
  c.config().setHostname(QString::fromUtf8("freedb.freedb.org"));
  c.config().setPort(8880);
  c.config().setCacheLookupEnabled(false);
  c.config().setFreedbLookupEnabled(true);
  c.config().setMusicBrainzLookupEnabled(false);
  c.config().setFreedbLookupTransport(Lookup::CDDBP);

  TrackOffsetList list;

  list
    << 150
    << 106965
    << 127220
    << 151925
    << 176085
    << 234500;

  Result r = c.lookup(list);

  QVERIFY(r == Success);

  CDInfoList response = c.lookupResponse();

  CDInfoList::ConstIterator it;

  bool hasRunTest = false;

  for (it = response.constBegin(); it != response.constEnd(); ++it)
  {
    CDInfo i(*it);

    if (i.get(QString::fromUtf8("discid")) == QString::fromUtf8("3e0c3a05") && i.get(Category) == QString::fromUtf8("misc"))
    {
      // If revision doesn't match, test probably needs to be updated
      // See: http://www.freedb.org/freedb/misc/3e0c3a05 for updated data
      QCOMPARE(i.get(QString::fromUtf8("revision")).toInt(), 17);

      QCOMPARE(i.get(Artist).toString(),QString::fromUtf8("Pink Floyd"));
      QCOMPARE(i.get(Title).toString(),QString::fromUtf8("Atom Heart Mother"));
      QCOMPARE(i.get(Genre).toString(),QString::fromUtf8("Psychedelic Rock"));
      QCOMPARE(i.get(Year).toInt(),1970);
      QCOMPARE(i.track(0).get(Title).toString(),QString::fromUtf8("Atom Heart Mother: Father's Shout/Breast Milky/Mother Fore/Funky Dung/Mind Your Throats Please/Remergence"));
      QCOMPARE(i.track(1).get(Title).toString(),QString::fromUtf8("If"));
      QCOMPARE(i.track(2).get(Title).toString(),QString::fromUtf8("Summer '68"));
      QCOMPARE(i.track(3).get(Title).toString(),QString::fromUtf8("Fat Old Sun"));
      QCOMPARE(i.track(4).get(Title).toString(),QString::fromUtf8("Alan's Psychedelic Breakfast: Rise and Shine/Sunny Side Up/Morning Glory"));
      QCOMPARE(i.track(0).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(1).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(2).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(3).get(Comment).toString(),QString::fromUtf8(""));
      QCOMPARE(i.track(4).get(Comment).toString(),QString::fromUtf8(""));
      for (int j=0; j <= 4; j++)
      {
        QCOMPARE(i.track(j).get(Artist).toString(),QString::fromUtf8("Pink Floyd"));
      }

      hasRunTest = true;
    }
  }

  QVERIFY(hasRunTest);
}

QTEST_GUILESS_MAIN(SyncCDDBLookupTest)
