/*
    SPDX-FileCopyrightText: 2006 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef ASYNCMUSICBRAINZLOOKUP_H
#define ASYNCMUSICBRAINZLOOKUP_H

#include "lookup.h"

namespace KCDDB
{
  class LookupThread;

  class AsyncMusicBrainzLookup : public Lookup
  {

    Q_OBJECT

    public:

      AsyncMusicBrainzLookup();
      virtual ~AsyncMusicBrainzLookup();

      Result lookup( const QString &, uint, const TrackOffsetList & ) override;

      CDInfoList lookupResponse() const;

    Q_SIGNALS:

      void finished( KCDDB::Result );

    protected Q_SLOTS:
      void lookupFinished( );

    private:
      LookupThread* m_lookupThread;
  };
}

#endif // ASYNCMUSICBRAINZLOOKUP_H

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
