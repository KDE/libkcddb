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

#ifndef KHTTP_ASYNC_HTTP_LOOKUP_H
#define KHTTP_ASYNC_HTTP_LOOKUP_H

#include <kurl.h>

#include <libkcddb/defines.h>
#include <libkcddb/asynclookup.h>
#include <libkcddb/cdinfo.h>

namespace KIO
{
  class Job;
  class TransferJob;
};

namespace KCDDB
{
  class AsyncHTTPLookup : public AsyncLookup
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

      AsyncHTTPLookup(QObject * parent = 0, const char * name = 0);

      virtual ~AsyncHTTPLookup();

      void lookup
        (
          const TrackOffsetList &,
          const QString         & hostname,
          uint                    port,
          const QString         & clientName,
          const QString         & clientVersion
        );

    signals:

      void finished
        (
          Lookup::Result,
          const CDInfoList & = CDInfoList()
        );

    protected slots:

      void slotData(KIO::Job *, const QByteArray &);
      void slotDataReq(KIO::Job *, QByteArray &);
      void slotDataReq(KIO::Job *, const KURL &);
      void slotResult(KIO::Job *);
      void slotCancelled(KIO::Job *);

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

    private:

      State               state_;
      TrackOffsetList     trackOffsetList_;
      KIO::TransferJob *  job_;
      QString             hostname_;
      uint                port_;
      QString             clientName_;
      QString             clientVersion_;
      bool                readOnly_;
      QStringList         cdInfoBuffer_;
      CDDBMatchList       matchList_;

      CDInfoList  cdInfoList_;
  };
}

#endif // KHTTP_ASYNC_HTTP_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
