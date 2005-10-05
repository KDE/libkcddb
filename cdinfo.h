/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002-2005 Benjamin Meyer <ben-devel@meyerhome.net>
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
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
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
  /**
   * Information about a sepecific track in a cd.
   */
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
       * @return a CDDB compatible string of all the data assigned to this track
       * tracknumber must be assigned before calling this.
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

  /**
   * Information about a CD
   *
   * Typically CDInfo is obtained from the client such as:
   * <code>KCDDB::Client *cddb = new KCDDB::Client();
   * cddb->lookup(discSignature);
   * CDInfo info = cddb->bestLookupResponse();</code>
   */
  class KDE_EXPORT CDInfo
  {
    public:
      CDInfo();
      virtual ~CDInfo();
      
      CDInfo(const CDInfo& clone);
      CDInfo& operator=(const CDInfo& clone);

      /**
       * Load CDInfo from a string that is CDDB compatible
       * @return true if successful
       */
      bool load(const QString &string);
      /**
       * Load CDInfo from a stringList that is CDDB compatible
       * @return true if successful
       */
      bool load(const QStringList &stringList);

      /**
       * Clear all information, setting this to invalid
       * internal
       */
      void clear();
      
      /**
       * @return true if the cd information is valid
       */
      bool isValid() const;

      /**
       * @param submit If submit is true only returns CDDB compatible information
       * @return a string containing all of the CD's information. 
       */
      QString toString(bool submit=false) const;
      
      /**
       * Get data for type that has been assigned to this disc.
       * @type is case insensitive.
       * For example <code>get("title")</code>
       */
      QVariant get(const QString &type) const;
      
      /**
       * Set any data from this disc.
       * @type is case insensitive.
       * For example <code>set("title", "Rock this world")</code>
       * Usefull for atributes that other apps want to add.
       * Data will be stored in the local cddb cache, but not sent to the cddb server
       */
      void set(const QString &type, const QVariant &data);
      
      uint          revision;
      TrackInfoList trackInfoList;

    protected:
      /**
       * @returns a valid CDDB line made up of name and value
       */
      QString createLine(const QString& name, const QString& value) const;
      /**
       * Checks to make sure that trackNumber exists
       */
      void checkTrack( int trackNumber );
      /**
       * escape's string for CDDB processing
       */
      static QString escape( const QString &string );
      /**
       * fixes an escaped string that has been CDDB processed
       */
      static QString unescape( const QString &string );
     
     private:
      class CDInfoPrivate *d;
  };

  typedef QList<CDInfo> CDInfoList;
}

#endif // KCDDB_CDINFO_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
