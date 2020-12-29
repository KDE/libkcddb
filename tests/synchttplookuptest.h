/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SYNCHTTPLOOKUPTEST_H
#define SYNCHTTPLOOKUPTEST_H

#include <QObject>

class SyncHTTPLookupTest : public QObject
{
    Q_OBJECT
  private Q_SLOTS:
    void testLookup();
};

#endif
