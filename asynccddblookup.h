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

#include <qsocket.h>

#include <libkcddb/defines.h>
#include <libkcddb/config.h>
#include <libkcddb/asynclookup.h>

namespace KCDDB
{
  class AsyncCDDBLookup : public AsyncLookup
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
        WaitingForQueryResponse,
        WaitingForMoreMatches,
        WaitingForCDInfoResponse,
        WaitingForCDInfoData
      };

      AsyncCDDBLookup(QObject * parent = 0, const char * name = 0);

      virtual ~AsyncCDDBLookup();

      virtual void lookup
        (
          const TrackOffsetList &,
          const QString         & hostname,
          uint                    port,
          const QString         & clientName,
          const QString         & clientVersion
        );

    protected slots:

      void slotConnected();
      void slotConnectionClosed();
      void slotError(int error);
      void slotReadyRead();

    protected:

      bool    parseGreeting(const QString &);
      void    sendHandshake();
      bool    parseHandshake(const QString &);
      void    sendQuery();
      bool    parseQueryResponse(const QString &);
      void    parseMatch(const QString &);
      void    requestCDInfoForMatch();
      bool    parseCDInfoResponse(const QString &);
      void    parseCDInfoData();
      void    read();

      QString readLine();

      QString stateToString() const;

    signals:

      void finished
        (
          Lookup::Result,
          const QValueList<CDInfo> & = QValueList<CDInfo>()
        );

    private:

      State           state_;
      TrackOffsetList trackOffsetList_;
      QSocket         socket_;
      QString         hostname_;
      uint            port_;
      QString         clientName_;
      QString         clientVersion_;
      bool            readOnly_;
      QStringList     cdInfoBuffer_;
      CDDBMatchList   matchList_;

      QValueList<CDInfo>  cdInfoList_;
  };
}

#endif // KCDDB_ASYNC_CDDB_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
