/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MUSICBRAINZTESTFULLDATE_H
#define MUSICBRAINZTESTFULLDATE_H

#include <QObject>

class MusicBrainzTestFullDate : public QObject
{
  Q_OBJECT
  private Q_SLOTS:
    void testLookup();
};

#endif
