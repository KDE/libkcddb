/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002-2005 Benjamin C. Meyer <ben at meyerhome dot net>
  Copyright (C) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>

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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef KCDDB_CLIENT_H
#define KCDDB_CLIENT_H

#include "kcddbconfig.h"
#include "cdinfo.h"
#include <QObject>
#include "kcddb.h"
#include <QtGlobal>

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
   * CDInfo info = cddb->lookupResponse().first();</code>
   */
  class KCDDB_EXPORT Client : public QObject
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
       * Searches the database for entries matching the offset list.
       * Use lookupResponse() to get the results
       *
       * @param trackOffsetList A List of the start offsets of the tracks,
       * and the offset of the lead-out track at the end of the list
       *
       * @return if the results of the lookup: Success, NoRecordFound, etc
       */
      Result lookup(const TrackOffsetList &trackOffsetList);
      /**
       * @returns the results of trying to submit
       */
      Result submit(const CDInfo &cdInfo, const TrackOffsetList &trackOffsetList);

      /**
       * Stores the CD-information in the local cache
       */
      void store(const CDInfo &cdInfo, const TrackOffsetList &trackOffsetList);

      void setBlockingMode( bool );
      bool blockingMode() const;

    Q_SIGNALS:
      /**
       * emitted when not blocking and lookup() finished.
       */
      void finished( KCDDB::Result result );

    protected Q_SLOTS:
      /**
       * Called when the lookup is finished with the result
       */
      void slotFinished( KCDDB::Result result );
      /**
       * Called when the submit is finished with the result
       */
      void slotSubmitFinished( KCDDB::Result result );

    private:
      Result runPendingLookups();

      class Private;
      Private * const d;
  };
}

#endif // KCDDB_CLIENT_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
