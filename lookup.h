/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  CopyRight (C) 2002 Nadeem Hasan <nhasan@kde.org>

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

#ifndef KCDDB_LOOKUP_H
#define KCDDB_LOOKUP_H

#include <qstring.h>
#include <qpair.h>
#include <qvaluelist.h>

#include <kextendedsocket.h>

#include "cdinfo.h"

namespace KCDDB
{
  typedef QPair<QString, QString> CDDBMatch;
  typedef QValueList<CDDBMatch> CDDBMatchList;
  typedef QValueList<uint> TrackOffsetList;

  class Lookup
  {
    public:

      enum Result
      {
        Success,
        ServerError,
        HostNotFound,
        NoResponse,
        NoRecordFound,
        MultipleRecordFound,
        CannotSave,
        UnknownError
      };

      enum Transport
      {
        CDDB,
        HTTP
      };

      Lookup();
      virtual ~Lookup();

      virtual Result lookup
        (
          const QString & hostName,
          const uint      port,
          const QString & clientName,
          const QString & clientVersion,
          const TrackOffsetList & 
        ) = 0;

      virtual CDInfoList lookupResponse() const = 0;

      static QString resultToString(Result);
      static QString transportToString(uint);
      static Transport stringToTransport(const QString &);
      static QString trackOffsetListToId( const TrackOffsetList& );

    protected:

      QString readLine();
      void writeLine( const QString & );

      bool parseGreeting( const QString& );
      bool parseHandshake( const QString& );
      void parseExtraMatch( const QString& );
      Result parseQuery( const QString& );

      QString trackOffsetListToId();
      QString trackOffsetListToString();
 
      KExtendedSocket socket_;

      QString user_;
      QString clientName_;
      QString clientVersion_;

      bool readOnly_;

      CDInfoList cdInfoList_;
      CDDBMatchList matchList_;
      TrackOffsetList trackOffsetList_;
  };
}

#endif // KCDDB_LOOKUP_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
