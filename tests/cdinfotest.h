/*
    SPDX-FileCopyrightText: 2005 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef CDINFOTEST_H
#define CDINFOTEST_H

#include <QObject>

class CDInfoTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testLongLines();
};

#endif
