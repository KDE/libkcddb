/*
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "httpsubmit.h"

#include <KIO/Job>

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
