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

#ifndef KCDDB_CDDB_H
#define KCDDB_CDDB_H

#include <qstring.h>
#include <qpair.h>
#include <qvaluelist.h>

#include <kdelibs_export.h>

/** The KCDDB namespace collects all the CDDB-related classes and methods. */
namespace KCDDB
{
   /** This list is used to calculate the CDDB disc id.
    Insert the start frames ((minute*60 + seconds)*75+frames)
    of all tracks, followed by the first frame of the disc and the last
    frame of the disc. The first frame is for most audio CD's the same
    as the first frame of the first track, the
    last one is the start frame of the leadout track.
    */
  typedef QValueList<uint> TrackOffsetList;

  /** This is just a container class used for interpreting results
   of CDDB queries.
   */
  class KDE_EXPORT CDDB
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
        InvalidCategory,
        UnknownError
      };

      CDDB();
      virtual ~CDDB();

      static QString resultToString(Result);
      static QString trackOffsetListToId( const TrackOffsetList & );

      static QString clientName() { return QString::fromLatin1("libkcddb"); }
      static QString clientVersion() { return QString::fromLatin1("0.21"); }

    protected:
      bool parseGreeting( const QString & );
      bool parseHandshake( const QString & );

      uint statusCode( const QString & );

      QString trackOffsetListToId();
      QString trackOffsetListToString();
 
      QString user_;
      QString localHostName_;

      bool readOnly_;

      TrackOffsetList trackOffsetList_;
  };
}

#endif // KCDDB_CDDB_H

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
