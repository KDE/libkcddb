/*
  Copyright (C) 2004-2006 Richard Lärkäng <nouseforaname@home.se>

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

#include "libkcddb/client.h"
#include "libkcddb/cache.h"
#include "libkcddb/lookup.h"
#include "utf8test.h"
#include <QtTest/QTest>

void Utf8Test::testLookup()
{
  using namespace KCDDB;

  Client c;
  c.config().setHostname("freedb.freedb.org");
  c.config().setPort(80);
  c.config().setCacheLookupEnabled(false);
  c.config().setFreedbLookupEnabled(true);
  c.config().setMusicBrainzLookupEnabled(false);
  c.config().setFreedbLookupTransport(Lookup::HTTP);

  TrackOffsetList list;

  list
  << 150
  << 61408
  << 177675;

  c.lookup(list);

  CDInfoList response = c.lookupResponse();

  CDInfoList::ConstIterator it;

  bool hasRunTest = false;

  for (it = response.constBegin(); it != response.constEnd(); ++it)
  {
    CDInfo i(*it);

    if (i.get("discid") == "13093f02" && i.get(Category) == "blues")
    {
      // If revision doesn't match, test probably needs to be updated
      // See: http://www.freedb.org/freedb/blues/13093f02 for updated data
      QCOMPARE(i.get("revision").toInt(), 3);

      QString artist = QString::fromUtf8("神城麻郁(浪川大輔)/宮藤深衣奈(中原麻衣)/小野寺樺恋(清水愛)/織部椿(根谷美智子)/島崎康生(鈴村健一)/風見みずほ(井上喜久子)/森野苺(田村ゆかり)/四道跨(三浦祥朗)/四道晴子(新谷良子)/佐川秋那(浅野真澄)/真下双葉(佐久間紅美)/菊池浩美(進藤尚美)/草薙桂(保志総一郎)/縁川小石(川澄綾子)/間雲漂介(岩田光央)/水澄楓(大原さやか)/まりえ(金田朋子)/山田政臣(杉田智和)神城麻郁(浪川大輔)/宮藤深衣奈(中原麻衣)/小野寺樺恋(清水愛)/織部椿(根谷美智子)/島崎康生(鈴村健一)/風見みずほ(井上喜久子)/森野苺(田村ゆかり)/四道跨(三Y祥朗)/四道晴子(新谷良子)/佐川秋那(浅野真澄)/真下双葉(佐久間紅美)/菊池浩美(進藤尚美)/草薙桂(保志総一郎)/縁川小石(川澄綾子)/間雲漂介(岩田光央)/水澄楓(大原さやか)/まりえ(金田朋子)/山田政臣(杉田智和)");

      QCOMPARE(i.get(Artist).toString(),artist);
      QCOMPARE(i.get(Title).toString(),QString::fromUtf8("みずほ先生とはちみつツインズ ドラマアルバム2時間目「百合百合ツインズ」"));
      QCOMPARE(i.get(Genre).toString(),QString("Anime"));
      QCOMPARE(i.get(Year).toInt(),2004);
      QCOMPARE(i.track(0).get(Title).toString(),QString::fromUtf8("第EX話 おねがい☆全員集合"));
      QCOMPARE(i.track(1).get(Title).toString(),QString::fromUtf8("エピローグ"));
      QCOMPARE(i.track(0).get(Comment).toString(),QString(""));
      QCOMPARE(i.track(1).get(Comment).toString(),QString(""));
      for (int j=0; j <= 1; j++)
      {
        QCOMPARE(i.track(j).get(Artist).toString(),artist);
      }

      hasRunTest = true;
    }
  }

  QVERIFY(hasRunTest);
}

QTEST_GUILESS_MAIN(Utf8Test)
