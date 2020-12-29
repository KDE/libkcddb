/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_ASYNC_CDDBP_LOOKUP_H
#define KCDDB_ASYNC_CDDBP_LOOKUP_H

#include "cddbplookup.h"

namespace KCDDB
{
  class AsyncCDDBPLookup : public CDDBPLookup 
  {
    Q_OBJECT

    public:

      enum State
      {
        Idle,
        WaitingForConnection,
        WaitingForGreeting,
        WaitingForHandshake,
        WaitingForProtoResponse,
        WaitingForQueryResponse,
        WaitingForMoreMatches,
        WaitingForCDInfoResponse,
        WaitingForCDInfoData,
        WaitingForQuitResponse
      };

      AsyncCDDBPLookup();

      virtual ~AsyncCDDBPLookup();

      Result lookup( const QString &, uint, const TrackOffsetList & ) override;

    Q_SIGNALS:

      void finished( KCDDB::Result );
      void quit( Result );

    protected Q_SLOTS:

      void slotGotError(QAbstractSocket::SocketError error);
      void slotConnectionSuccess();
      void slotReadyRead();

    protected:

      void doHandshake();
      void doProto();
      void doQuery();
      void doQuit();

      bool parseQueryResponse( const QString & );
      void requestCDInfoForMatch();
      bool parseCDInfoResponse( const QString & );
      void parseCDInfoData();

      void read();

      QString readLine();

      QString stateToString() const;

    private:

      State state_;
      Result result_;
      QStringList cdInfoBuffer_;
  };
}

#endif // KCDDB_ASYNC_CDDBP_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
