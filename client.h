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

#ifndef KCDDB_CLIENT_H
#define KCDDB_CLIENT_H

#include "config.h"
#include "cdinfo.h"
#include <qobject.h>
#include "cddb.h"

namespace KCDDB
{
  class Lookup;
  class Submit;

  class Client : public QObject
  {
    Q_OBJECT

    public:

      /**
       * Uses settings read from config.
       */
      Client();

      /**
       * Use custom settings.
       */
      Client(const Config &);

      virtual ~Client();

      const Config & config() const;

      CDInfoList lookupResponse() const;

      CDDB::Result lookup(const TrackOffsetList &);
      CDDB::Result submit(const CDInfo &, const TrackOffsetList &);

      void setBlockingMode( bool );
      bool blockingMode() const;

    signals:

      void finished( CDDB::Result );

    protected slots:
      void slotFinished( CDDB::Result );
      void slotSubmitFinished( CDDB::Result );

    private:
      class Private;
      Private * d;
      Lookup * cdInfoLookup;
      Submit * cdInfoSubmit;
  };
}

#endif // KCDDB_CLIENT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
