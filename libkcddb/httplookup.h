/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley ( rikkus ) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_HTTP_LOOKUP_H
#define KCDDB_HTTP_LOOKUP_H

#include "lookup.h"
#include <QUrl>

namespace KIO
{
  class Job;
}

namespace KCDDB
{
  class HTTPLookup : public Lookup
  {

    Q_OBJECT

    public:

      enum State
      {
        Idle,
        WaitingForQueryResponse,
        WaitingForReadResponse
      };

      HTTPLookup();
      virtual ~HTTPLookup();

    protected:

      void initURL( const QString &, uint );
      void makeURL( const QString & );
      virtual Result fetchURL() = 0;

      void jobFinished();

      Result sendQuery();
      Result sendRead( const CDDBMatch & );

    Q_SIGNALS:

      void queryReady();
      void readReady();

    protected:

      bool block_;
      QUrl cgiURL_;
      QByteArray data_;
      State state_;
      Result result_;
  };
}

#endif

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
