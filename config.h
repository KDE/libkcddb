/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002 Nadeem Hasan <nhasan@kde.org>

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

#ifndef KCDDB_CONFIG_H
#define KCDDB_CONFIG_H

#include "cache.h"
#include "cddb.h"

namespace KCDDB
{
  class Config
  {
    public:

      /**
       * Fill in from defaults.
       */
      Config();

      bool operator == (const Config & other) const;
      bool operator != (const Config & other) const;

      /**
       * @param filename Name of rc file, not full path.
       */
      void load(const QString & filename = "kcmcddbrc");

      /**
       * @param filename Name of rc file, not full path.
       */
      void save(const QString & filename = "kcmcddbrc");

      static QString  group();

      static QString  hostnameKey();
      static QString  portKey();
      static QString  submitTransportKey();
      static QString  lookupTransportKey();
      static QString  emailAddressKey();
      static QString  submissionsEnabledKey();
      static QString  cachePolicyKey();
      static QString  cacheLocationsKey();

      QString             hostname()            const;
      uint                port()                const;
      CDDB::Transport     submitTransport()     const;
      CDDB::Transport     lookupTransport()     const;
      QString             emailAddress()        const;
      bool                submissionsEnabled()  const;
      Cache::Policy       cachePolicy()         const;
      QStringList         cacheLocations()      const;
      QString             smtpHostName()        const;
      uint                smtpPort()            const;
      QString             smtpUsername()        const;
      QString             smtpPassword()        const;

      void setHostname            (const QString &);
      void setPort                (uint);
      void setSubmitTransport     (CDDB::Transport);
      void setLookupTransport     (CDDB::Transport);
      void setEmailAddress        (const QString &);
      void setSubmissionsEnabled  (bool);
      void setCachePolicy         (Cache::Policy);
      void setCacheLocations      (const QStringList &);
      void setSmtpHostName        (const QString &);
      void setSmtpPort            (uint);
      void setSmtpUsername        (const QString &);
      void setSmtpPassword        (const QString &);

    private:

      QString           hostname_;
      uint              port_;
      QString           user_;
      QString           clientName_;
      QString           clientVersion_;
      CDDB::Transport   submitTransport_;
      CDDB::Transport   lookupTransport_;
      QString           emailAddress_;
      bool              submissionsEnabled_;
      Cache::Policy     cachePolicy_;
      QStringList       cacheLocations_;
      QString           smtpHostName_;
      uint              smtpPort_;
      QString           smtpUsername_;
      QString           smtpPassword_;

  };
}

#endif // KCDDB_CONFIG_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
