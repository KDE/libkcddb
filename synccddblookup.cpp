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

#include <qstringlist.h>
#include <qdir.h>
#include <qfile.h>
#include <kdebug.h>
#include <qsocket.h>

#include <libkcddb/synccddblookup.h>
#include <libkcddb/client.h>

namespace KCDDB
{
  SyncCDDBLookup::SyncCDDBLookup()
  {
    // Empty.
  }

  SyncCDDBLookup::~SyncCDDBLookup()
  {
    // Empty.
  }

    CDInfoList
  SyncCDDBLookup::lookupResponse() const
  {
    return cdInfoList_;
  }

    Lookup::Result
  SyncCDDBLookup::lookup
  (
    const TrackOffsetList & trackOffsetList,
    const QString         & hostname,
    uint                    port,
    const QString         & clientName,
    const QString         & clientVersion
  )
  {
    clientName_     = clientName;
    clientVersion_  = clientVersion;

    // Connect to server.

    kdDebug()
      << "Trying to connect to "
      << hostname
      << ":"
      << port
      << endl;

    Connect::Result connectResult = connectSocket(socket_, hostname, port);

    switch (connectResult)
    {
      case Connect::Success:
        break;

      case Connect::HostNotFound:
        return Lookup::HostNotFound;
        break;

      case Connect::NoResponse:
        return Lookup::NoResponse;

      default:
        return Lookup::UnknownError;
    }

    kdDebug() << "Connected" << endl;

    // Check welcome message.

    if (!serverWelcomeOk())
      return NoResponse;

    // Try a handshake.

    if (!shakeHands())
      return NoResponse;

    // Run a query.

    CDDBMatchList matchList = runQuery(trackOffsetList);

    if (matchList.isEmpty())
      return NoRecordFound;

    kdDebug() << matchList.count() << " matches saved" << endl;

    // For each match, read the cd info from the server and save it to
    // cdInfoList.

    if (!getMatchesToCDInfoList(matchList))
      return Lookup::UnknownError; // XXX Do we need a ServerError ?
 
    writeLine(socket_, "quit");

    socket_.close();

    return Lookup::Success;
  }

    bool
  SyncCDDBLookup::serverWelcomeOk()
  {
    QString line = readLine(socket_);

    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    if (200 == serverStatus)
    {
      kdDebug() << "Server response: read-only" << endl;
    }
    else if (201 == serverStatus)
    {
      kdDebug() << "Server response: read-write" << endl;
    }
    else
    {
      kdDebug() << "Server response: bugger off" << endl;
      return false;
    }

    return true;
  }

    bool
  SyncCDDBLookup::shakeHands()
  {
    QString handshake = "cddb hello ";
    handshake += user_;
    handshake += " ";
    handshake += "localhost"; // FIXME
    handshake += " ";
    handshake += clientName_;
    handshake += " ";
    handshake += clientVersion_;

    writeLine(socket_, handshake);

    QString line = readLine(socket_);

    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    if ((200 != serverStatus) && (402 != serverStatus))
    {
      kdDebug() << "Handshake was too tight. Letting go." << endl;
      return false;
    }

    kdDebug() << "Handshake successful" << endl;

    return true;
  }

    CDDBMatchList
  SyncCDDBLookup::runQuery(const TrackOffsetList & offsetList)
  {
    CDDBMatchList matchList;

    QString query = "cddb query ";
    query += trackOffsetListToId(offsetList);
    query += " ";
    query += trackOffsetListToString(offsetList);

    writeLine(socket_, query);

    QString line = readLine(socket_);

    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    kdDebug() << "Server status: " << serverStatus << endl;

    if (200 == serverStatus)
    {
      kdDebug() << "Server found exact match" << endl;
      matchList.append(qMakePair(tokenList[1], tokenList[2]));
    }
    else if (211 == serverStatus)
    {
      kdDebug() << "Server found inexact matches" << endl;

      line = readLine(socket_);

      while ('.' != line[0])
      {
        tokenList = QStringList::split(' ', line);
        matchList.append(qMakePair(tokenList[0], tokenList[1]));
        line = readLine(socket_);
      }
    }
    else if (210 == serverStatus)
    {
      kdDebug() << "Server found multiple exact matches" << endl;

      line = readLine(socket_);

      while ('.' != line[0])
      {
        tokenList = QStringList::split(' ', line);
        matchList.append(qMakePair(tokenList[0], tokenList[1]));
        line = readLine(socket_);
      }
    }
    else
    {
      kdDebug() << "Server said error" << endl;
    }

    return matchList;
  }

    bool
  SyncCDDBLookup::getMatchesToCDInfoList(const CDDBMatchList & matchList)
  {
    // XXX Always returns true. Hmm...

    CDDBMatchList::ConstIterator matchIt;

    for (matchIt = matchList.begin(); matchIt != matchList.end(); ++matchIt)
      (void) getMatchToCDInfoList(*matchIt);

    return true;
  }

    bool
  SyncCDDBLookup::getMatchToCDInfoList(const CDDBMatch & match)
  {
    QString category  = match.first;
    QString discid    = match.second;

    kdDebug() << "Match: " << category << " : " << discid << endl;

    QString readRequest = "cddb read ";
    readRequest += category;
    readRequest += " ";
    readRequest += discid;

    writeLine(socket_, readRequest);

    QStringList lineList;

    QString line = readLine(socket_);

    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    if (210 != serverStatus)
    {
      kdDebug() << "Server error !" << endl;
      return false;
    }

    line = readLine(socket_);

    while ('.' != line[0])
    {
      lineList.append(line);
      line = readLine(socket_);
    }

    CDInfo info;

    if (info.load(lineList))
      cdInfoList_.append(info);

    return true;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
