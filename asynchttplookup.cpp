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

#include <kdebug.h>

#include <libkcddb/asynchttplookup.h>

namespace KCDDB
{
  AsyncHTTPLookup::AsyncHTTPLookup(QObject * parent, const char * name)
    : QObject   (parent, name),
      state_    (Idle),
      job_      (0),
      port_     (0)
  {
  }

  AsyncHTTPLookup::~AsyncHTTPLookup()
  {
    delete job_;
  }

    void
  AsyncHTTPLookup::lookup
  (
    const TrackOffsetList & trackOffsetList,
    const QString         & hostname,
    uint                    port,
    const QString         & clientName,
    const QString         & clientVersion
  )
  {
    trackOffsetList_  = trackOffsetList;
    hostname_         = hostname;
    port_             = port;
    clientName_       = clientName;
    clientVersion_    = clientVersion;
    readOnly_         = true;

    state_ = WaitingForHostResolution;

    kdDebug() << "Asking socket to connect to "
      << hostname_ << ":" << port << endl;

    QString query = "TODO";

    job_ = KIO::get("http://" + hostname_ + ":" + QString::number(port)
      + "/~cddb/cddb.cgi/" + query);

    if (0 == job_)
    {
      kdDebug() << "Can't create TransferJob" << endl;
      emit(error(Unknown));
      state_ = Idle;
      return;
    }

    connect
      (
        job_,
        SIGNAL(data(KIO::Job *, const QByteArray &)),
        SLOT(slotData(KIO::Job *, const QByteArray &))
      );

    connect
      (
        job_,
        SIGNAL(dataReq(KIO::Job *, QByteArray &)),
        SLOT(slotDataReq(KIO::Job *, QByteArray &))
      );

    connect
      (
        job_,
        SIGNAL(redirection(KIO::Job *, const KURL &)),
        SLOT(slotDataReq(KIO::Job *, const KURL &))
      );

    connect
      (
        job_,
        SIGNAL(result(KIO::Job *)),
        SLOT(slotResult(KIO::Job *))
      );

    connect
      (
        job_,
        SIGNAL(canceled(KIO::Job *)),
        SLOT(slotCancelled(KIO::Job *))
      );
  }

    void
  AsyncHTTPLookup::slotData(KIO::Job *, const QByteArray & data)
  {
    kdDebug() << k_funcinfo << endl;

    QString s(data);

    kdDebug() << s << endl;
  }

    void
  AsyncHTTPLookup::slotDataReq(KIO::Job *, QByteArray &)
  {
    kdDebug() << k_funcinfo << endl;
  }

    void
  AsyncHTTPLookup::slotDataReq(KIO::Job *, const KURL &)
  {
    kdDebug() << k_funcinfo << endl;
  }

    void
  AsyncHTTPLookup::slotResult(KIO::Job *)
  {
    kdDebug() << k_funcinfo << endl;
  }

    void
  AsyncHTTPLookup::slotCancelled(KIO::Job *)
  {
    kdDebug() << k_funcinfo << endl;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
