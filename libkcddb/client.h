/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002-2005 Benjamin C. Meyer <ben at meyerhome dot net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_CLIENT_H
#define KCDDB_CLIENT_H

#include "cdinfo.h"
#include "kcddb.h"
#include "config.h"

#include <QObject>

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
