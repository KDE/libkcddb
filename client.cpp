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
#include <kextsock.h>
#include <kdebug.h>
#include <qsocket.h>

#include <libkcddb/client.h>

namespace KCDDB
{
  class Client::Private
  {
    public:

      Config config;

      QValueList<CDInfo> cdInfoList;

      QString            cddbId;

      KExtendedSocket    socket;
  };

  static Error connectSocket
    (
      KExtendedSocket & socket,
      const QString   & hostname,
      uint              port
    )
  {
    socket.setHost(hostname);
    socket.setPort(port);

    int lookupReturn = socket.lookup();

    if (0 != lookupReturn)
      return HostNotFound;

    int connectReturn = socket.connect();

    if (0 != connectReturn)
      return NoResponse;

    return None;
  }

  Client::Client()
  {
    d = new Private;

    d->config.load();
  }

  Client::Client(const Config & config)
  {
    d = new Private;

    d->config = config;
  }

  Client::~Client()
  {
    delete d;
  }

    Config
  Client::config() const
  {
    return d->config;
  }

    QValueList<CDInfo>
  Client::lookupResponse() const
  {
    return d->cdInfoList;
  }

    Error
  Client::lookup(const TrackOffsetList & trackOffsetList)
  {
    // Get the cddb id from trackOffsetList.

    d->cddbId = trackOffsetListToId(trackOffsetList);
    kdDebug() << k_funcinfo << "cddbId: " << d->cddbId << endl;

    if (!d->cddbId)
    {
      kdDebug() << "Can't create cddbid from offset list" << endl;
      return NoSuchCD;
    }

    // Try the cache if we're supposed to.

    if (
      CDDBLookupIgnoreCached != d->config.lookupTransport()
      &&
      HTTPLookupIgnoreCached != d->config.lookupTransport()
    )
    {
      kdDebug() << "Should try cache here" << endl;

      QFile f(QDir::homeDirPath() + "/.cddb/" + d->cddbId);

      if (f.exists())
      {
        // STUB
        kdDebug() << "Found locally cached info" << endl;
        return Unknown;
      }

      kdDebug() << "Didn't find locally cached info" << endl;
    }

    // If we're only supposed to try the cache and we failed, drop out now.

    if (CacheOnlyLookup == d->config.lookupTransport())
    {
      kdDebug() << "Only trying cache. Give up now." << endl;
      return NoSuchCD;
    }

    // Do the actual lookup.

    switch (d->config.lookupTransport())
    {
      case CDDBLookup:
      case CDDBLookupIgnoreCached:
        return cddbLookup(trackOffsetList);
        break;

      case HTTPLookup:
      case HTTPLookupIgnoreCached:
        return httpLookup(trackOffsetList);
        break;

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << lookupTransportToString(d->config.lookupTransport()) << endl;
        return Unknown;
        break;
    }
  }

    Error
  Client::submit(const CDInfo &cdInfo)
  {
    // Do CannotSave sinarios
    if(cdInfo.id == "0")
      return CannotSave;
    for (uint i=0; i < cdInfo.trackInfoList.count(); i++)
      if(!cdInfo.trackInfoList[i].offsetKnown)
        return CannotSave;

QString diskData = "";
diskData += "# xmcd\n";
diskData += "#\n";
diskData += "# Track frame offsets:\n";
for (uint i=0; i < cdInfo.trackInfoList.count(); i++){
  diskData += QString("#\t%1\n").arg(cdInfo.trackInfoList[i].offset);
}
diskData += "#\n";
diskData += QString("DISCID=%1\n").arg(cdInfo.id);
diskData += QString("DTITLE=%1\n").arg(cdInfo.title);
diskData += QString("DYEAR=%1\n").arg(cdInfo.year);
diskData += QString("DGENRE=%1\n").arg(cdInfo.genre);
for (uint i=0; i < cdInfo.trackInfoList.count(); i++){
  diskData += QString("TTITLE%1=%2\n").arg(i).arg(cdInfo.trackInfoList[i].title);
}

    switch (d->config.submitTransport())
    {
      case HTTPSubmit:
      {

#define sendOut(a) submitSocket.writeBlock( a.latin1(), a.length() );

QSocket submitSocket(0, "http Sumbition Socket");
submitSocket.connectToHost("www.freecddb.com", 80);
sendOut(QString("POST /submit.cgi HTTP/1.0\n"));
sendOut(QString("Category: %1\n").arg(cdInfo.genre));
sendOut(QString("Discid: %1\n").arg(cdInfo.id));
sendOut(QString("User-Email: ben@meyerhome.net\n"));
sendOut(QString("Submit-Mode: test\n")); // Change to "submit"
sendOut(QString("Charset: ISO-8859-1\n"));
sendOut(QString("X-Cddbd-Note: Sent by libkcddb - Questions: ben@meyerhome.net.\n"));
sendOut(QString("Content-Length: %1\n").arg(diskData.length())); // Get real length()
sendOut(QString("\n"));
sendOut(diskData);

#undef sendOut
return None;

      }
      break;
      
      case SMTPSubmit:
      {
        return CannotSave;
      }
      break;

      default:
        kdDebug() << k_funcinfo << "Unsupported transport: "
          << submitTransportToString(d->config.submitTransport()) << endl;
        return Unknown;
        break;
    }
  }

    Error
  Client::cddbLookup(const TrackOffsetList & offsetList)
  {
    // Connect to server.

    kdDebug()
      << "Trying to connect to "
      << d->config.hostname()
      << ":"
      << d->config.port()
      << endl;

    Error connectError =
      connectSocket(d->socket, d->config.hostname(), d->config.port());

    if (None != connectError)
      return connectError;

    kdDebug() << "Connected" << endl;

    // Check welcome message.

    if (!cddbServerWelcomeOk())
      return NoResponse;

    // Try a handshake.

    if (!cddbShakeHands())
      return NoResponse;

    // Run a query.

    CDDBMatchList matchList = cddbRunQuery(offsetList);

    if (matchList.isEmpty())
      return NoSuchCD;

    kdDebug() << matchList.count() << " matches saved" << endl;

    // For each match, read the cd info from the server and save it to
    // d->cdInfoList.

    if (!cddbGetMatchesToCDInfoList(matchList))
      return Unknown; // XXX Do we need a ServerError ?
 
    writeLine("quit");

    d->socket.close();

    return None;
  }

    Error
  Client::httpLookup(const TrackOffsetList & offsetList)
  {
    kdDebug() << "Trying to connect to " << d->config.hostname()
      << ":" << d->config.port() << endl;

    Error connectError =
      connectSocket(d->socket, d->config.hostname(), d->config.port());

    if (None != connectError)
      return connectError;

    kdDebug() << "Connected" << endl;

    kdDebug() << "STUB" << endl;

    d->socket.disconnect();

    return Unknown;
  }

    QString
  Client::readLine()
  {
    kdDebug() << k_funcinfo << endl;

    if (220 != d->socket.socketStatus())
    {
      kdDebug() << "socket status: " << d->socket.socketStatus() << endl;
      return QString::null;
    }

    QCString buf;

    int c = d->socket.getch();

    while ('\n' != c)
    {
      buf += c;
      c = d->socket.getch();
    }

    kdDebug() << "READ: `" << buf << "'" << endl;
    return QString::fromLatin1(buf.data(), buf.length());
  }

    void
  Client::writeLine(const QString & s)
  {
    kdDebug() << k_funcinfo << endl;

    if (220 != d->socket.socketStatus())
    {
      kdDebug() << "socket status: " << d->socket.socketStatus() << endl;
      return;
    }

    QCString buf = s.latin1();
    kdDebug() << "WRITE: `" << buf << "'" << endl;
    buf.append("\n");

    d->socket.writeBlock(buf.data(), buf.length());
  }

    bool
  Client::cddbServerWelcomeOk()
  {
    QString line = readLine();

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
  Client::cddbShakeHands()
  {
    QString handshake = "cddb hello ";
    handshake += d->config.user();
    handshake += " ";
    handshake += "localhost"; // FIXME
    handshake += " ";
    handshake += d->config.clientName();
    handshake += " ";
    handshake += d->config.clientVersion();

    writeLine(handshake);

    QString line = readLine();

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
  Client::cddbRunQuery(const TrackOffsetList & offsetList)
  {
    CDDBMatchList matchList;

    QString query = "cddb query ";
    query += d->cddbId;
    query += " ";
    query += trackOffsetListToString(offsetList);

    writeLine(query);

    QString line = readLine();

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

      line = readLine();

      while ('.' != line[0])
      {
        tokenList = QStringList::split(' ', line);
        matchList.append(qMakePair(tokenList[0], tokenList[1]));
        line = readLine();
      }
    }
    else if (210 == serverStatus)
    {
      kdDebug() << "Server found multiple exact matches" << endl;

      line = readLine();

      while ('.' != line[0])
      {
        tokenList = QStringList::split(' ', line);
        matchList.append(qMakePair(tokenList[0], tokenList[1]));
        line = readLine();
      }
    }
    else
    {
      kdDebug() << "Server said error" << endl;
    }

    return matchList;
  }

    bool
  Client::cddbGetMatchesToCDInfoList(const CDDBMatchList & matchList)
  {
    // XXX Always returns true. Hmm...

    CDDBMatchList::ConstIterator matchIt;

    for (matchIt = matchList.begin(); matchIt != matchList.end(); ++matchIt)
      (void) cddbGetMatchToCDInfoList(*matchIt);

    return true;
  }

    bool
  Client::cddbGetMatchToCDInfoList(const CDDBMatch & match)
  {
    QString category  = match.first;
    QString discid    = match.second;

    kdDebug() << "Match: " << category << " : " << discid << endl;

    QString readRequest = "cddb read ";
    readRequest += category;
    readRequest += " ";
    readRequest += discid;

    writeLine(readRequest);

    QStringList lineList;

    QString line = readLine();

    QStringList tokenList = QStringList::split(' ', line);

    uint serverStatus = tokenList[0].toUInt();

    if (210 != serverStatus)
    {
      kdDebug() << "Server error !" << endl;
      return false;
    }

    line = readLine();

    while ('.' != line[0])
    {
      lineList.append(line);
      line = readLine();
    }

    d->cdInfoList.append(parseStringListToCDInfo(lineList));

    return true;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
