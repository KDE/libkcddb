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

#include <libkcddb/asynccddblookup.h>

namespace KCDDB
{
  AsyncCDDBLookup::AsyncCDDBLookup(QObject * parent, const char * name)
    : QObject(parent, name),
      state_(Idle),
      port_(0)
  {
    connect
      (
        &socket_,
        SIGNAL(lookupFinished(int)),
        SLOT(slotLookupFinished(int))
      );

    connect
      (
        &socket_,
        SIGNAL(connectionSuccess()),
        SLOT(slotConnectionSuccess())
      );

    connect
      (
        &socket_,
        SIGNAL(connectionFailed(int)),
        SLOT(slotConnectionFailed(int))
      );

    connect
      (
        &socket_,
        SIGNAL(readyRead()),
        SLOT(slotReadyRead())
      );
  }

  AsyncCDDBLookup::~AsyncCDDBLookup()
  {
    // Empty.
  }

    void
  AsyncCDDBLookup::lookup
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

    socket_.setAddress(hostname_, port_);
    socket_.setTimeout(60);
    socket_.setBlockingMode(false);

    state_ = WaitingForHostResolution;

    socket_.startAsyncLookup();
  }

    void
  AsyncCDDBLookup::slotLookupFinished(int hostCount)
  {
    kdDebug() << k_funcinfo << endl;
    if (0 == hostCount)
    {
      emit(error(HostNotFound));
      state_ = Idle;
      return;
    }

    state_ = WaitingForConnection;
    socket_.startAsyncConnect();
  }

    void
  AsyncCDDBLookup::slotConnectionSuccess()
  {
    kdDebug() << k_funcinfo << endl;
    state_ = WaitingForGreeting;
  }

    void
  AsyncCDDBLookup::slotConnectionFailed(int /* Who cares ? */)
  {
    kdDebug() << k_funcinfo << endl;
    emit(error(NoResponse));
    state_ = Idle;
    return;
  }

    void
  AsyncCDDBLookup::slotReadyRead()
  {
    kdDebug() << k_funcinfo << endl;

    switch (state_)
    {
      case WaitingForGreeting:
        break;

      default:
        break;
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
