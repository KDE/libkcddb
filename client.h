/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002-2005 Benjamin C. Meyer <ben at meyerhome dot net>

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
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef KCDDB_CLIENT_H
#define KCDDB_CLIENT_H

#include "config.h"
#include "cdinfo.h"
#include <qobject.h>
#include "cddb.h"
#include <kdemacros.h>

namespace KCDDB
{
  class Lookup;
  class Submit;

  /**
   * Class used to obtain CDDB information about a CD
   * 
   * Example:
   * <code>KCDDB::Client *cddb = new KCDDB::Client();
   * cddb->lookup(discSignature);
   * CDInfo info = cddb->bestLookupResponse();</code>
   */
  class KDE_EXPORT Client : public QObject
  {
    Q_OBJECT

    public:

      /**
       * Uses settings read from config.
       */
      Client();

      virtual ~Client();

      Config & config() const;

      /**
       * @return a list of CDDB entries that match the disc signature
       */
      CDInfoList lookupResponse() const;
      /**
       * @return a the CDDB entries that is the best match to the disc signature
       */
      CDInfo bestLookupResponse() const;

      /**
       * @return if the results of the lookup: Success, NoRecordFound, etc
       */
      CDDB::Result lookup(const TrackOffsetList &trackOffsetList);
      /**
       * @returns the results of trying to submit
       */
      CDDB::Result submit(const CDInfo &cdInfo, const TrackOffsetList &trackOffsetList);

      void setBlockingMode( bool );
      bool blockingMode() const;

    signals:
      /**
       * emited when not blocking and lookup() finished.
       */
      void finished( CDDB::Result result );

    protected slots:
      /**
       * Called when the lookup is finished with the result
       */ 
      void slotFinished( CDDB::Result result );
      /**
       * Called whtne the submit is finished with the result
       */
      void slotSubmitFinished( CDDB::Result result );

    private:
      class Private;
      Private * d;
      
      Lookup * cdInfoLookup;
      Submit * cdInfoSubmit;
  };
}

#endif // KCDDB_CLIENT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
