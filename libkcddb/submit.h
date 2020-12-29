/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002 Nadeem Hasan <nhasan@kde.org>
    SPDX-FileCopyrightText: 2005 Richard Lärkäng <nouseforaname@home.se>e>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_SUBMIT_H
#define KCDDB_SUBMIT_H

#include "cddb.h"
#include "cdinfo.h"
#include <QObject>

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
