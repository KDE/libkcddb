/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MUSICBRAINZTEST_H
#define MUSICBRAINZTEST_H

#include <QObject>

class MusicBrainzTest : public QObject
{
  Q_OBJECT
  private Q_SLOTS:
    void testLookup();
};

#endif
