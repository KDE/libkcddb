/*
    SPDX-FileCopyrightText: 2005 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MUSICBRAINZLOOKUP_H
#define MUSICBRAINZLOOKUP_H

#include "../lookup.h"
#include "../cdinfo.h"
#include "../kcddb.h"
#include "../config.h"

namespace MusicBrainz5
{
  class CArtistCredit;
}

namespace KCDDB
{
  class MusicBrainzLookup : public Lookup
  {
    Q_OBJECT

    public:

      MusicBrainzLookup();
      virtual ~MusicBrainzLookup();

      // FIXME Only freedb lookup needs the first two arguments (host/port)
      Result lookup( const QString &, uint, const TrackOffsetList & ) override;

      static CDInfoList cacheFiles(const TrackOffsetList &, const Config& );

    private:

      static QString calculateDiscId(const TrackOffsetList & );
      static QString artistFromCreditList(MusicBrainz5::CArtistCredit * );
  } ;
}

#endif // MUSICBRAINZ_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
