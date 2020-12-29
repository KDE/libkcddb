/*
    SPDX-FileCopyrightText: 2005 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SYNCCDDBLOOKUPTEST_H
#define SYNCCDDBLOOKUPTEST_H

#include <QObject>

class SyncCDDBLookupTest : public QObject
{
    Q_OBJECT
  private Q_SLOTS:
    void testLookup();
};

#endif
