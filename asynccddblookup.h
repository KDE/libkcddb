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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#ifndef KCDDB_ASYNC_CDDB_LOOKUP_H
#define KCDDB_ASYNC_CDDB_LOOKUP_H

#include <qobject.h>

#include "cddblookup.h"

namespace KCDDB
{
  class AsyncCDDBLookup : public QObject, public CDDBLookup 
  {
    Q_OBJECT

    public:

      enum State
      {
        Idle,
        WaitingForHostResolution,
        WaitingForConnection,
        WaitingForGreeting,
        WaitingForHandshake,
        WaitingForProtoResponse,
        WaitingForQueryResponse,
        WaitingForMoreMatches,
        WaitingForCDInfoResponse,
        WaitingForCDInfoData
      };

      AsyncCDDBLookup( QObject * parent = 0, const char * name = 0 );

      virtual ~AsyncCDDBLookup();

      Result lookup( const QString &, uint, const QString &,
          const QString &, const TrackOffsetList & );

    signals:

      void finished( Lookup::Result );

    protected slots:

      void slotLookupFinished( int );
      void slotConnectionSuccess();
      void slotConnectionFailed( int );
      void slotReadyRead();

    protected:

      void sendHandshake();
      void sendProto();
      void sendQuery();
      bool parseQueryResponse( const QString & );
      void requestCDInfoForMatch();
      bool parseCDInfoResponse( const QString & );
      void parseCDInfoData();
      void read();

      QString stateToString() const;

    private:

      State state_;
      QStringList cdInfoBuffer_;
  };
}

#endif // KCDDB_ASYNC_CDDB_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
