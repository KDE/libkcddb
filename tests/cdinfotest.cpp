/*
  Copyright (C) 2005-2006 Richard Lärkäng <nouseforaname@home.se>

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

#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "libkcddb/cdinfo.h"
#include "cdinfotest.h"

using namespace KCDDB;

void CDInfoTest::testLongLines()
{
    CDInfo info;

    QString str;

    TrackInfo& tinfo = info.track(0);

    for (int i=0; i <= 100; i++)
    {
        str.fill(QLatin1Char('A'),10*i);
        info.set(Title, str);
        str.fill(QLatin1Char('B'),10*i);
        str.append(QLatin1Char('\n'));
        info.set(Comment, str);
        str.fill(QLatin1Char('C'),10*i);
        info.set(Artist, str);
        str.fill(QLatin1Char('D'),10*i);
        info.set(Genre, str);
        str.fill(QLatin1Char('E'),10*i);
        info.set(Length, str);
        str.fill(QLatin1Char('F'),10*i);
        str.append(QLatin1Char('\n'));
        info.set(QString::fromUtf8("custom"), str);
        str.fill(QLatin1Char('G'),10*i);
        tinfo.set(Title, str);
        str.fill(QLatin1Char('H'),10*i);
        str.append(QLatin1Char('\n'));
        tinfo.set(QString::fromUtf8("custom"), str);
        QString data = info.toString();
        QString submitData = info.toString(true);

        CDInfo info2;
        info2.load(data);
        str.fill(QLatin1Char('A'),10*i);
        QCOMPARE(info2.get(Title).toString(), str);
        str.fill(QLatin1Char('B'),10*i);
        str.append(QLatin1Char('\n'));
        QCOMPARE(info2.get(Comment).toString(), str);
        str.fill(QLatin1Char('C'),10*i);
        QCOMPARE(info2.get(Artist).toString(), str);
        str.fill(QLatin1Char('D'),10*i);
        if (i == 0)
            QCOMPARE(info2.get(Genre).toString(), QString::fromUtf8("Unknown"));
        else
            QCOMPARE(info2.get(Genre).toString(), str);
        str.fill(QLatin1Char('E'),10*i);
        QCOMPARE(info2.get(Length).toString(), str);
        str.fill(QLatin1Char('F'),10*i);
        str.append(QLatin1Char('\n'));
        QCOMPARE(info2.get(QString::fromUtf8("custom")).toString(), str);
        str.fill(QLatin1Char('G'),10*i);
        QCOMPARE(info2.track(0).get(Title).toString(), str);
        const CDInfo info3(info2);
        QCOMPARE(info3.track(0).get(Title).toString(), str);
        str.fill(QLatin1Char('H'),10*i);
        str.append(QLatin1Char('\n'));
        QCOMPARE(info2.track(0).get(QString::fromUtf8("custom")).toString(), str);

        QVERIFY(!submitData.contains(QString::fromUtf8("CUSTOM")));
        QVERIFY(data.contains(QString::fromUtf8("CUSTOM")));
    }
}

QTEST_GUILESS_MAIN(CDInfoTest)
