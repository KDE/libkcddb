/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef UTF8TEST_H
#define UTF8TEST_H

#include <QObject>

class Utf8Test : public QObject
{
    Q_OBJECT
  private Q_SLOTS:
    void testLookup();
};

#endif
