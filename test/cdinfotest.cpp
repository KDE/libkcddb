/*
  Copyright (C) 2005-2006 Richard Lärkäng <nouseforaname@home.se>
  Copyright (C) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>

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
        str.fill('A',10*i);
        info.set(Title, str);
        str.fill('B',10*i);
        str.append('\n');
        info.set(Comment, str);
        str.fill('C',10*i);
        info.set(Artist, str);
        str.fill('D',10*i);
        info.set(Genre, str);
        str.fill('E',10*i);
        info.set(Length, str);
        str.fill('F',10*i);
        str.append('\n');
        info.set("custom", str);
        str.fill('G',10*i);
        tinfo.set(Title, str);
        str.fill('H',10*i);
        str.append('\n');
        tinfo.set("custom", str);
        QString data = info.toString();
        QString submitData = info.toString(true);

        CDInfo info2;
        info2.load(data);
        str.fill('A',10*i);
        QCOMPARE(info2.get(Title).toString(), str);
        str.fill('B',10*i);
        str.append('\n');
        QCOMPARE(info2.get(Comment).toString(), str);
        str.fill('C',10*i);
        QCOMPARE(info2.get(Artist).toString(), str);
        str.fill('D',10*i);
        if (i == 0)
            QCOMPARE(info2.get(Genre).toString(), QString("Unknown"));
        else
            QCOMPARE(info2.get(Genre).toString(), str);
        str.fill('E',10*i);
        QCOMPARE(info2.get(Length).toString(), str);
        str.fill('F',10*i);
        str.append('\n');
        QCOMPARE(info2.get("custom").toString(), str);
        str.fill('G',10*i);
        QCOMPARE(info2.track(0).get(Title).toString(), str);
        const CDInfo info3(info2);
        QCOMPARE(info3.track(0).get(Title).toString(), str);
        str.fill('H',10*i);
        str.append('\n');
        QCOMPARE(info2.track(0).get("custom").toString(), str);

        QVERIFY(!submitData.contains("CUSTOM"));
        QVERIFY(data.contains("CUSTOM"));
    }
}

QTEST_MAIN(CDInfoTest)

#include "cdinfotest.moc"
