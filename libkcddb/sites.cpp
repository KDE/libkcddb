/*
    SPDX-FileCopyrightText: 2004 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sites.h"

#include <KIO/TransferJob>
#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>
#include <QUrl>
#include <QUrlQuery>

namespace KCDDB
{
  Sites::Sites()
  {

  }

    QList<Mirror>
  Sites::siteList()
  {
    QUrl url;
    url.setScheme( QLatin1String( "http" ) );
    url.setHost( QLatin1String( "gnudb.gnudb.org" ) );
    url.setPort( 80 );
    url.setPath( QLatin1String( "/~cddb/cddb.cgi" ) );

    QString hello = QString::fromLatin1("%1 %2 %3 %4")
        .arg(QLatin1String( "libkcddb-user" ), QLatin1String( "localHost" ), CDDB::clientName(), CDDB::clientVersion());

    QUrlQuery query;
    query.addQueryItem( QLatin1String( "cmd" ), QLatin1String( "sites" ) );
    query.addQueryItem( QLatin1String( "hello" ), hello );
    query.addQueryItem( QLatin1String( "proto" ), QLatin1String( "5" ) );
    url.setQuery( query );

    QList<Mirror> result;

    KIO::TransferJob* job = KIO::get( url, KIO::NoReload, KIO::HideProgressInfo );
    QByteArray data;
    QObject::connect( job, &KIO::TransferJob::data, [&data](KIO::Job *, const QByteArray &d){ data += d; } );
    if( job->exec() )
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
      if (line == QLatin1String( "." ))
        break;
      result << parseLine(line);
    }

    return result;
  }

    Mirror
  Sites::parseLine(const QString& line)
  {
    Mirror m;

    const QRegularExpression rexp(QLatin1String( "([^ ]+) (cddbp|http) (\\d+) ([^ ]+) [N|S]\\d{3}.\\d{2} [E|W]\\d{3}.\\d{2} (.*)" ));

    if (const auto match = rexp.match(line); match.hasMatch())
    {
      m.address = match.captured(1);

      if (match.capturedView(2) == QLatin1String( "cddbp" ))
        m.transport = Lookup::CDDBP;
      else
        m.transport = Lookup::HTTP;

      m.port = match.capturedView(3).toUInt();

      if (m.transport == Lookup::HTTP && match.capturedView(4) != QLatin1String( "/~cddb/cddb.cgi" ))
        qWarning() << "Non default urls are not supported for http";

      m.description = match.captured(5);
    }

    return m;
  }
}
