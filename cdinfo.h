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

#ifndef KCDDB_CDINFO_H
#define KCDDB_CDINFO_H

#include <qstringlist.h>
#include <qvaluelist.h>

namespace KCDDB
{
  class TrackInfo
  {
    public:

      TrackInfo();
      ~TrackInfo();

      uint    offset;
      QString title;
      QString extt;
  };

  typedef QValueList<TrackInfo> TrackInfoList;

  class CDInfo
  {
    public:

      CDInfo();
      ~CDInfo();

      bool load(const QString &);
      bool load(const QStringList &);

      void clear();

      bool isValid()
      {
        return !id.isEmpty() && !trackInfoList.isEmpty();
      }

      QString toString() const;

      QString       id;
      QString       artist;
      QString       title;
      QString       genre;
      QString       extd;
      uint          year;
      uint          length;
      TrackInfoList trackInfoList;

    protected:

      void checkTrack( uint );
      QString escape( const QString & ) const;
      QString unescape( const QString & ) const;
  };

  typedef QValueList<CDInfo> CDInfoList;
}

#endif // KCDDB_CDINFO_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
