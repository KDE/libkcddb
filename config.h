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

#ifndef KCDDB_CONFIG_H
#define KCDDB_CONFIG_H

#include <libkcddb/defines.h>
#include <libkcddb/submit.h>
#include <libkcddb/lookup.h>
#include <libkcddb/cache.h>

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
      static QString  userKey();
      static QString  clientNameKey();
      static QString  clientVersionKey();
      static QString  submitTransportKey();
      static QString  lookupTransportKey();
      static QString  proxyHostnameKey();
      static QString  proxyPortKey();
      static QString  smtpHostnameKey();
      static QString  smtpPortKey();
      static QString  emailAddressKey();
      static QString  proxyEnabledKey();
      static QString  submissionsEnabledKey();

      QString             hostname()            const;
      uint                port()                const;
      QString             user()                const;
      QString             clientName()          const;
      QString             clientVersion()       const;
      Submit::Transport   submitTransport()     const;
      Lookup::Transport   lookupTransport()     const;
      QString             proxyHostname()       const;
      uint                proxyPort()           const;
      QString             smtpHostname()        const;
      uint                smtpPort()            const;
      QString             emailAddress()        const;
      bool                proxyEnabled()        const;
      bool                submissionsEnabled()  const;
      Cache::Policy       cachePolicy()         const;

      void setHostname            (const QString &);
      void setPort                (uint);
      void setUser                (const QString &);
      void setClientName          (const QString &);
      void setClientVersion       (const QString &);
      void setSubmitTransport     (Submit::Transport);
      void setLookupTransport     (Lookup::Transport);
      void setProxyHostname       (const QString &);
      void setProxyPort           (uint);
      void setSMTPHostname        (const QString &);
      void setSMTPPort            (uint);
      void setEmailAddress        (const QString &);
      void setProxyEnabled        (bool);
      void setSubmissionsEnabled  (bool);
      void setCachePolicy         (Cache::Policy);

    private:

      QString           hostname_;
      uint              port_;
      QString           user_;
      QString           clientName_;
      QString           clientVersion_;
      Submit::Transport submitTransport_;
      Lookup::Transport lookupTransport_;
      QString           proxyHostname_;
      uint              proxyPort_;
      QString           smtpHostname_;
      uint              smtpPort_;
      QString           emailAddress_;
      bool              proxyEnabled_;
      bool              submissionsEnabled_;
      Cache::Policy     cachePolicy_;
  };
}

#endif // KCDDB_CONFIG_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
