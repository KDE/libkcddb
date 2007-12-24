/*
  Copyright ( C ) 2004 Richard Lärkäng <nouseforaname@home.se>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (  at your option ) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "sites.h"

#include <kurl.h>
#include <kio/netaccess.h>
#include <kio/job.h>
#include <QTextStream>
#include <kdebug.h>
#include <QRegExp>

namespace KCDDB
{
  Sites::Sites()
  {

  }

    QList<Mirror>
  Sites::siteList()
  {
    KUrl url;
    url.setProtocol( "http" );
    url.setHost( "freedb.freedb.org" );
    url.setPort( 80 );
    url.setPath( "/~cddb/cddb.cgi" );

    url.setQuery( QString::null );	//krazy:exclude=nullstrassign for old broken gcc

    QString hello = QString("%1 %2 %3 %4")
        .arg("libkcddb-user", "localHost", CDDB::clientName(), CDDB::clientVersion());

    url.addQueryItem( "cmd", "sites" );
    url.addQueryItem( "hello", hello );
    url.addQueryItem( "proto", "5" );

    QList<Mirror> result;

    KIO::Job* job = KIO::get( url, KIO::NoReload, KIO::HideProgressInfo );
    QByteArray data;
    if( KIO::NetAccess::synchronousRun( job, 0, &data ) )
    {
      result = readData( data );
    }

    return result;
  }

    QList<Mirror>
  Sites::readData(const QByteArray& data)
  {
    QList<Mirror> result;

    QTextStream ts(data);

    if (CDDB::statusCode(ts.readLine()) != 210)
      return result;

    while (!ts.atEnd())
    {
      QString line = ts.readLine();
      if (line == ".")
        break;
      result << parseLine(line);
    }

    return result;
  }

    Mirror
  Sites::parseLine(const QString& line)
  {
    Mirror m;

    QRegExp rexp("([^ ]+) (cddbp|http) (\\d+) ([^ ]+) [N|S]\\d{3}.\\d{2} [E|W]\\d{3}.\\d{2} (.*)");

    if (rexp.indexIn(line) != -1)
    {
      m.address = rexp.cap(1);

      if (rexp.cap(2) == "cddbp")
        m.transport = Lookup::CDDBP;
      else
        m.transport = Lookup::HTTP;

      m.port = rexp.cap(3).toUInt();

      if (m.transport == Lookup::HTTP && rexp.cap(4) != "/~cddb/cddb.cgi")
        kWarning() << "Non default urls are not supported for http";

      m.description = rexp.cap(5);
    }

    return m;
  }
}
