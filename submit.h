/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>
  Copyright (C) 2005 Richard Lärkäng <nouseforaname@home.se>e>

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

#ifndef KCDDB_SUBMIT_H
#define KCDDB_SUBMIT_H

#include "cddb.h"
#include "cdinfo.h"
#include <qobject.h>

namespace KIO
{
  class Job;
}

namespace KCDDB
{
  class Submit : public CDDB, public QObject
  {
    public:

      enum Transport
      {
        HTTP,
        SMTP
      };

      Submit();
      virtual ~Submit();

      Result submit( CDInfo cdInfo, const TrackOffsetList &offsetList);

    protected:
      virtual KIO::Job* createJob(const CDInfo& cdInfo) = 0;
      virtual Result runJob(KIO::Job* job) = 0;
    
      bool validCategory(const QString&);

      Result parseWrite( const QString & );
      virtual void makeDiskData( const CDInfo&, const TrackOffsetList& );
      QString diskData_;
  };
}

#endif // KCDDB_SUBMIT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
