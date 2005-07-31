/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002-2004 Nadeem Hasan <nhasan@nadmm.com>

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

#ifndef KCDDB_CDINFO_H
#define KCDDB_CDINFO_H

#include <qstringlist.h>
#include <qlist.h>
#include <kdemacros.h>
#include <qvariant.h>

namespace KCDDB
{
  class KDE_EXPORT TrackInfo
  {
    public:
      /**
       * The most common types
       */
      enum Type
      {
        Title,
        Comment,
        Artist,
        Genre,
        Year,
        Length,
        Category,
        Extt
      };
    
      TrackInfo();
      virtual ~TrackInfo();
      TrackInfo(const TrackInfo& clone);
      TrackInfo& operator=(const TrackInfo& clone);
      
      /**
       * Get data for type that has been assigned to this track.
       * @type is case insensitive.
       * For example <code>get("title")</code>
       */
      QVariant get(const QString &type) const;
      /**
       * Helper function that calls type with the common name
       */
      QVariant get(Type type) const;
      
      /**
       * Set any data from this track.
       * @type is case insensitive.
       * For example <code>set("title", "Rock this world")</code>
       * Usefull for atributes that other apps want to add.
       * Data will be stored in the local cddb cache, but not sent to the cddb server
       */
      void set(const QString &type, const QVariant &data);

      /**
       * @returns a CDDB compatible string of all the data assigned to this track
       * tracknumber should be assigned before calling this.
       */
      QString toString() const;
      
      /**
       * internal
       */
      void clear();
      
    private:
      class TrackInfoPrivate *d;

  };

  typedef QList<TrackInfo> TrackInfoList;

  class KDE_EXPORT CDInfo
  {
    public:

      CDInfo();
      virtual ~CDInfo();
      CDInfo(const CDInfo& clone);
      CDInfo& operator=(const CDInfo& clone);

      bool load(const QString &);
      bool load(const QStringList &);

      void clear();
      bool isValid() const;

      QString toString(bool submit=false) const;

      QVariant get(const QString &type) const;
      void set(const QString &type, const QVariant &data);
      
      uint          revision;
      TrackInfoList trackInfoList;

    protected:
      QString createLine(const QString& name, const QString& value) const;
      void checkTrack( uint );
      static QString escape( const QString & );
      static QString unescape( const QString & );
     
     private:
      class CDInfoPrivate *d;
  };

  typedef QList<CDInfo> CDInfoList;
}

#endif // KCDDB_CDINFO_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
