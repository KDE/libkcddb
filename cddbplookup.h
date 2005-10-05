/*
  Copyright (C) 2002 Rik Hemsley ( rikkus ) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or ( at your option ) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef KCDDB_CDDBP_LOOKUP_H
#define KCDDB_CDDBP_LOOKUP_H

#include <kbufferedsocket.h>

#include "lookup.h"

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
      Q_LONG writeLine( const QString & );

      bool isConnected()
        { return KNetwork::KClientSocketBase::Connected == socket_->state(); }

      KNetwork::KBufferedSocket* socket_;
  };
}

#endif

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
