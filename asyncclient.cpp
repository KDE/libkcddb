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

#include <libkcddb/asyncclient.h>

namespace KCDDB
{
  class AsyncClient::Private
  {
    public:

      Config              config;
      QValueList<CDInfo>  lookupResponse;
  };

  AsyncClient::AsyncClient(QObject * parent, const char * name)
    : QObject(parent, name)
  {
    d = new Private;
  }

  AsyncClient::AsyncClient
    (
      const Config  & config,
      QObject       * parent,
      const char    * name
    )
    : QObject(parent, name)
  {
    d = new Private;
    d->config = config;
  }

  AsyncClient::~AsyncClient()
  {
    delete d;
  }

    Config
  AsyncClient::config() const
  {
    return d->config;
  }

    QValueList<CDInfo>
  AsyncClient::lookupResponse() const
  {
    return d->lookupResponse;
  }

    Error
  AsyncClient::lookup(const TrackOffsetList &)
  {
    return Unknown;
  }

    Error
  AsyncClient::submit(const CDInfo &)
  {
    return Unknown;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
