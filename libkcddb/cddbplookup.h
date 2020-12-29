/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley ( rikkus ) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_CDDBP_LOOKUP_H
#define KCDDB_CDDBP_LOOKUP_H

#include "lookup.h"

#include <QTcpSocket>

namespace KCDDB
{
  class CDDBPLookup : public Lookup
  {
    public:
      CDDBPLookup();
      virtual ~CDDBPLookup();

      void sendHandshake();
      void sendProto();
      void sendQuery();
      void sendRead( const CDDBMatch &  );
      void sendQuit();

      void close();
    protected:
      qint64 writeLine( const QString & );

      bool parseGreeting( const QString & );
      bool parseHandshake( const QString & );

      bool isConnected()
        { return QAbstractSocket::ConnectedState == socket_->state(); }

      QTcpSocket* socket_;
  };
}

#endif

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
