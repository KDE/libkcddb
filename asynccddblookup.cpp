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

#include "asynccddblookup.h"

namespace KCDDB
{
  AsyncCDDBLookup::AsyncCDDBLookup(QObject * parent, const char * name)
    : AsyncLookup (parent, name),
      state_      (Idle),
      port_       (0)
  {
    connect
      (
        &socket_,
        SIGNAL(connected()),
        SLOT(slotConnected())
      );

    connect
      (
        &socket_,
        SIGNAL(connectionClosed()),
        SLOT(slotConnectionClosed())
      );

    connect
      (
        &socket_,
        SIGNAL(error(int)),
        SLOT(slotError(int))
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
    readOnly_         = true;

    state_ = WaitingForHostResolution;

    kdDebug() << "Asking socket to connect to "
      << hostname_ << ":" << port << endl;

    socket_.connectToHost(hostname_, port_);
  }

    void
  AsyncCDDBLookup::slotConnected()
  {
    kdDebug() << k_funcinfo << endl;
    state_ = WaitingForGreeting;
  }

    void
  AsyncCDDBLookup::slotConnectionClosed()
  {
    kdDebug() << k_funcinfo << endl;
    if (state_ != Idle)
      emit(finished(NoResponse));
    state_ = Idle;
    return;
  }

    void
  AsyncCDDBLookup::slotReadyRead()
  {
    kdDebug() << k_funcinfo << endl;
    kdDebug() << "State: " << stateToString() << endl;

    while (socket_.canReadLine())
      read();
  }

    void
  AsyncCDDBLookup::read()
  {
    switch (state_)
    {
      case WaitingForGreeting:

        if (!parseGreeting(readLine()))
        {
          emit(finished(ServerHatesUs));
          state_ = Idle;
          return;
        }
        else
        {
          sendHandshake();
        }
        break;

      case WaitingForHandshake:

        if (!parseHandshake(readLine()))
        {
          emit(finished(ServerHatesUs));
          state_ = Idle;
          return;
        }
        else
        {
          sendQuery();
        }
        break;

      case WaitingForQueryResponse:

        if (!parseQueryResponse(readLine()))
        {
          emit(finished(ServerHatesUs));
          state_ = Idle;
          return;
        }
        else
        {
          kdDebug()  << "Setting state to WaitingForMoreMatches" << endl;
          state_ = WaitingForMoreMatches;
        }
        break;

      case WaitingForMoreMatches:

        {
          QString line = readLine();

          if ('.' == line[0])
          {
            requestCDInfoForMatch();
          }
          else
          {
            parseMatch(line);
          }
        }

        break;

      case WaitingForCDInfoResponse:

        if (parseCDInfoResponse(readLine()))
        {
          state_ = WaitingForCDInfoData;
        }
        else
        {
          requestCDInfoForMatch();
        }

        break;

      case WaitingForCDInfoData:

        {
          QString line = readLine();

          if ('.' == line[0])
          {
            parseCDInfoData();
            requestCDInfoForMatch();
          }
          else
          {
            cdInfoBuffer_ << line;
          }
        }

        break;

      default:
        break;
    }
  }

    bool
  AsyncCDDBLookup::parseGreeting(const QString & line)
  {
    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    if (200 == serverStatus)
    {
      kdDebug() << "Server response: read-only" << endl;
    }
    else if (201 == serverStatus)
    {
      kdDebug() << "Server response: read-write" << endl;
      readOnly_ = false;
    }
    else
    {
      kdDebug() << "Server response: bugger off" << endl;
      return false;
    }

    return true;
  }

    void
  AsyncCDDBLookup::sendHandshake()
  {
    QTextStream str(&socket_);

    QString handshake = "cddb hello ";
    handshake += "libkcddb-user";
    handshake += " ";
    handshake += "localhost"; // FIXME
    handshake += " ";
    handshake += clientName_;
    handshake += " ";
    handshake += clientVersion_;

    str << handshake << "\r\n";

    state_ = WaitingForHandshake;
  }

    bool
  AsyncCDDBLookup::parseHandshake(const QString & line)
  {
    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    if ((200 != serverStatus) && (402 != serverStatus))
    {
      kdDebug() << "Handshake was too tight. Letting go." << endl;
      return false;
    }

    kdDebug() << "Handshake was warm and firm" << endl;

    return true;
  }

    void
  AsyncCDDBLookup::sendQuery()
  {
    QString query = "cddb query ";
    query += trackOffsetListToId(trackOffsetList_);
    query += " ";
    query += trackOffsetListToString(trackOffsetList_);

    QTextStream str(&socket_);

    str << query << "\r\n";

    state_ = WaitingForQueryResponse;
  }

    bool
  AsyncCDDBLookup::parseQueryResponse(const QString & line)
  {
    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    kdDebug() << "Server status: " << serverStatus << endl;

    if (200 == serverStatus)
    {
      kdDebug() << "Server found exact match" << endl;
      matchList_.append(qMakePair(tokenList[1], tokenList[2]));
    }
    else if (211 == serverStatus)
    {
      kdDebug() << "Server found inexact matches" << endl;
    }
    else if (210 == serverStatus)
    {
      kdDebug() << "Server found multiple exact matches" << endl;
    }
    else
    {
      kdDebug() << "Server said no matches or error" << endl;
      return false;
    }

    return true;
  }

    void
  AsyncCDDBLookup::parseMatch(const QString & line)
  {
    QStringList tokenList = QStringList::split(' ', line);
    matchList_.append(qMakePair(tokenList[0], tokenList[1]));
  }

    void
  AsyncCDDBLookup::requestCDInfoForMatch()
  {
    if (matchList_.isEmpty())
    {
      emit(finished(Success, cdInfoList_));
      state_ = Idle;
      return;
    }

    CDDBMatch match = matchList_.first();

    matchList_.remove(matchList_.front());

    QString category  = match.first;
    QString discid    = match.second;

    QTextStream str(&socket_);

    kdDebug() << "Match: " << category << " : " << discid << endl;

    QString readRequest = "cddb read ";
    readRequest += category;
    readRequest += " ";
    readRequest += discid;

    str << readRequest << "\r\n";

    state_ = WaitingForCDInfoResponse;
  }

    bool
  AsyncCDDBLookup::parseCDInfoResponse(const QString & line)
  {
    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    if (210 != serverStatus)
    {
      kdDebug() << "Server error !" << endl;
      return false;
    }

    return true;
  }

    void
  AsyncCDDBLookup::parseCDInfoData()
  {
    CDInfo info;

    if (info.load(cdInfoBuffer_))
      cdInfoList_.append(info);
  }

    void
  AsyncCDDBLookup::slotError(int err)
  {
    kdDebug() << "Socket error: " << err << endl;
    if (state_ != Idle)
    {
      emit(finished(NoResponse));
      state_ = Idle;
    }
  }

    QString
  AsyncCDDBLookup::readLine()
  {
    QString line = socket_.readLine();

    kdDebug() << "Read: `" << line << "'" << endl;

    return line;
  }

    QString
  AsyncCDDBLookup::stateToString() const
  {
    switch (state_)
    {
      case Idle:
        return "Idle";
        break;

      case WaitingForHostResolution:
        return "WaitingForHostResolution";
        break;

      case WaitingForConnection:
        return "WaitingForConnection";
        break;

      case WaitingForGreeting:
        return "WaitingForGreeting";
        break;

      case WaitingForHandshake:
        return "WaitingForHandshake";
        break;

      case WaitingForQueryResponse:
        return "WaitingForQueryResponse";
        break;

      case WaitingForMoreMatches:
        return "WaitingForMoreMatches";
        break;

      case WaitingForCDInfoResponse:
        return "WaitingForCDInfoResponse";
        break;

      case WaitingForCDInfoData:
        return "WaitingForCDInfoData";
        break;

      default:
        return "Unknown";
        break;
    }
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
