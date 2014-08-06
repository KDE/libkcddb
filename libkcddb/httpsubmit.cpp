/*
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2003 Richard Lärkäng <nouseforaname@home.se>

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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "httpsubmit.h"

#include <KIO/Job>
#include <QtCore/QDebug>

namespace KCDDB
{
  HTTPSubmit::HTTPSubmit(const QString& from, const QString& hostname, uint port)
    : Submit(), from_(from)
  {
    url_.setScheme( QLatin1String( "http" ));
    url_.setHost(hostname);
    url_.setPort(port);
    url_.setPath(QLatin1String( "/~cddb/submit.cgi" ));
  }

  HTTPSubmit::~HTTPSubmit()
  {

  }

  KIO::Job* HTTPSubmit::createJob(const CDInfo& cdInfo)
  {
    KIO::TransferJob* job = KIO::http_post(url_, diskData_.toUtf8().data(), KIO::HideProgressInfo);

    job->addMetaData(QLatin1String( "content-type" ), QLatin1String( "Content-Type: text/plain" ));
    QString header;

    header += QLatin1String( "Content-Type: text/plain\n" );

    header += QLatin1String( "Category: " ) + cdInfo.get(Category).toString() + QLatin1Char( '\n' );
    header += QLatin1String( "Discid: " ) + cdInfo.get(QLatin1String( "discid" )).toString() + QLatin1Char( '\n' );
    header += QLatin1String( "User-Email: " ) + from_ + QLatin1Char( '\n' );
    //header += QLatin1String( "Submit-Mode: test\n" );
    header += QLatin1String( "Submit-Mode: submit\n" );
    header += QLatin1String( "Charset: UTF-8" );

    job->addMetaData(QLatin1String( "customHTTPHeader" ), header);

    return job;
  }
}
