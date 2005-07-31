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
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "sites.h"
#include <kurl.h>
#include <kio/netaccess.h>
#include <qfile.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <QTextStream>
#include <kdebug.h>
#include <qregexp.h>

namespace KCDDB
{
  Sites::Sites()
    : CDDB()
  {

  }

    Q3ValueList<Mirror>
  Sites::siteList()
  {
    KURL url;
    url.setProtocol( "http" );
    url.setHost( "freedb.freedb.org" );
    url.setPort( 80 );
    url.setPath( "/~cddb/cddb.cgi" );

    url.setQuery( QString::null );

    QString hello = QString("%1 %2 %3 %4")
        .arg(user_, localHostName_, clientName(), clientVersion());

    url.addQueryItem( "cmd", "sites" );
    url.addQueryItem( "hello", hello );
    url.addQueryItem( "proto", "5" );

    Q3ValueList<Mirror> result;

    QString tmpFile;
    if( KIO::NetAccess::download( url, tmpFile, 0 ) )
    {
      result = readFile( tmpFile );
      KIO::NetAccess::removeTempFile( tmpFile );
    }

    return result;
  }

    Q3ValueList<Mirror>
  Sites::readFile(const QString& fileName)
  {
    Q3ValueList<Mirror> result;

    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly))
    {
      kdDebug(60010) << "Couldn't read: " << fileName << endl;
      return result;
    }

    QTextStream ts(&f);

    if (statusCode(ts.readLine()) != 210)
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

    if (rexp.search(line) != -1)
    {
      m.address = rexp.cap(1);

      if (rexp.cap(2) == "cddbp")
        m.transport = Lookup::CDDBP;
      else
        m.transport = Lookup::HTTP;

      m.port = rexp.cap(3).toUInt();

      if (m.transport == Lookup::HTTP && rexp.cap(4) != "/~cddb/cddb.cgi")
        kdWarning() << "Non default urls are not supported for http" << endl;
     
      m.description = rexp.cap(5);
    }

    return m;
  }
}
