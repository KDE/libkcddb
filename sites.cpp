#include "sites.h"
#include <kurl.h>
#include <kio/netaccess.h>
#include <qfile.h>
#include <kdebug.h>

namespace KCDDB
{
  Sites::Sites()
    : CDDB()
  {

  }

    QStringList
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

    QStringList result;

    QString tmpFile;
    if( KIO::NetAccess::download( url, tmpFile, 0 ) )
    {
      result = readFile( tmpFile );
      KIO::NetAccess::removeTempFile( tmpFile );
    }

    return result;
  }

    QStringList
  Sites::readFile(const QString& fileName)
  {
    QStringList result;

    QFile f(fileName);
    if (!f.open(IO_ReadOnly))
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
      result << line;
    }

    return result;
  }
}
