/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>

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

#ifndef KCDDB_ASYNC_HTTP_LOOKUP_H
#define KCDDB_ASYNC_HTTP_LOOKUP_H

#include "httplookup.h"

class KJob;

namespace KCDDB
{
  class AsyncHTTPLookup : public HTTPLookup
  {

    Q_OBJECT

    public:

      AsyncHTTPLookup();
      virtual ~AsyncHTTPLookup();

      Result lookup( const QString &, uint, const TrackOffsetList & ) override;

      CDInfoList lookupResponse() const;

    Q_SIGNALS:

      void finished( KCDDB::Result );

    protected Q_SLOTS:
      void slotQueryReady();
      void requestCDInfoForMatch();

      void slotData( KIO::Job *, const QByteArray & );
      void slotResult( KJob * );

    protected:
      Result fetchURL() override;

      Result runQuery();
  };
}

#endif // KCDDB_ASYNC_HTTP_LOOKUP_H

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
