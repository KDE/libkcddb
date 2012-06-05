/*
  Copyright (C) 2005 Richard Lärkäng <nouseforaname@home.se>

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

#ifndef MUSICBRAINZLOOKUP_H
#define MUSICBRAINZLOOKUP_H

#include "../lookup.h"
#include "../cdinfo.h"
#include "../kcddb.h"
#include "../kcddbconfig.h"

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
      virtual Result lookup( const QString &, uint, const TrackOffsetList & );

      static CDInfoList cacheFiles(const TrackOffsetList &, const Config& );

    private:

      static QString calculateDiscId(const TrackOffsetList & );
      static QString artistFromCreditList(MusicBrainz5::CArtistCredit * );
  } ;
}

#endif // MUSICBRAINZ_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
