/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MUSICBRAINZTESTSEVERALDISCS_H
#define MUSICBRAINZTESTSEVERALDISCS_H

#include <QObject>

class MusicBrainzTestSeveralDiscs : public QObject
{
  Q_OBJECT
  public:
    static void initMain();
  private Q_SLOTS:
    void testLookup();
};

#endif
