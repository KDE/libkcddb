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

#ifndef KCDDB_ASYNC_CLIENT_H
#define KCDDB_ASYNC_CLIENT_H

#include <qobject.h>

#include <libkcddb/defines.h>
#include <libkcddb/config.h>
#include <libkcddb/lookup.h>
#include <libkcddb/cdinfo.h>

namespace KCDDB
{
  class AsyncClient : public QObject
  {
    Q_OBJECT

    public:

      /**
       * Uses settings read from config.
       */
      AsyncClient(QObject * parent = 0, const char * name = 0);

      /**
       * Use custom settings.
       */
      AsyncClient(const Config &, QObject * parent = 0, const char * name = 0);

      virtual ~AsyncClient();

      Config config() const;

      void lookup(const TrackOffsetList &);
      void submit(const CDInfo &);

    signals:

      void submitComplete();

      void result
        (Lookup::Result, const CDInfoList & = CDInfoList());

    protected:

      void lookupWithHelper(const TrackOffsetList &);

    protected slots:

      void slotLookupFinished(Lookup::Result, const CDInfoList &);

    private:

      class Private;
      Private * d;
  };
}

#endif // KCDDB_ASYNC_CLIENT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
