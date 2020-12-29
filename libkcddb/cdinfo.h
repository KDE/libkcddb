/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002-2005 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2002-2004 Nadeem Hasan <nhasan@nadmm.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_CDINFO_H
#define KCDDB_CDINFO_H

#include <kcddb_export.h>
#include <QStringList>
#include <QVariant>

namespace KCDDB
{
  /**
   * The most common types
   */
  enum Type
  {
    Title, /**< The title of the track or CD */
    Comment, /**< A comment for the track or CD */
    Artist, /**< The artist of the track or CD */
    Genre, /**< The genre of the track or CD */
    Year, /**< The year the cd or track was produced
               By default, the year of the track is the
               same as for the whole cd
               @todo Doesn't do that for tracks right now.*/
    Length, /**< The length of a track or a full CD
                 @todo In what unit? */
    Category /**< The freedb category of the entry.
                  Needs to be one of: blues, classical,
                  country, data, fold, jazz, misc, newage,
                  reggae, rock, soundtrack */
  };

  /**
   * Information about a sepecific track in a cd.
   */
  class KCDDB_EXPORT TrackInfo
  {
    public:

      TrackInfo();
      virtual ~TrackInfo();
      TrackInfo(const TrackInfo& clone);
      TrackInfo& operator=(const TrackInfo& clone);

      bool operator==(const TrackInfo&) const;
      bool operator!=(const TrackInfo&) const;

      /**
       * Get data for type that has been assigned to this track.
       * @p type is case insensitive.
       * For example <code>get("title")</code>
       */
      QVariant get(const QString &type) const;
      /**
       * Helper function that calls type with the common name
       */
      QVariant get(Type type) const;

      /**
       * Set any data from this track.
       * @p type is case insensitive.
       * For example <code>set("title", "Rock this world")</code>
       * Useful for atributes that other apps want to add.
       * Data will be stored in the local cddb cache, but not sent to the cddb server
       */
      void set(const QString &type, const QVariant &data);
      /**
       * Helper function that calls type with the common name
       */
      void set(Type type, const QVariant &data);

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
   * CDInfo info = cddb->lookupResponse().first();</code>
   */
  class KCDDB_EXPORT CDInfo
  {
    public:
      CDInfo();
      virtual ~CDInfo();

      CDInfo(const CDInfo& clone);
      CDInfo& operator=(const CDInfo& clone);

      bool operator==(const CDInfo&) const;
      bool operator!=(const CDInfo&) const;

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
       * @p type is case insensitive.
       * For example <code>get("title")</code>
       */
      QVariant get(const QString &type) const;
      /**
       * Helper function that calls type with the common name
       */
      QVariant get(Type type) const;

      /**
       * Set any data from this disc.
       * @p type is case insensitive.
       * For example <code>set("title", "Rock this world")</code>
       * Useful for atributes that other apps want to add.
       * Data will be stored in the local cddb cache, but not sent to the cddb server
       */
      void set(const QString &type, const QVariant &data);
      /**
       * Helper function that calls type with the common name
       */
      void set(Type type, const QVariant &data);

      /**
       * Returns track with nr @p trackNumber and adds it to
       * the track list if it doesn't exist (first track is 0)
       */
      TrackInfo & track( int trackNumber );

      /**
       * Returns a const track with nr @p trackNumber
       * or a new if it doesn't exist (first track is 0)
       */
      TrackInfo track( int trackNumber ) const;

      /**
       * Returns number of tracks on CD
       */
      int numberOfTracks() const;

    protected:
      /**
       * Checks to make sure that trackNumber exists
       */
      void checkTrack( int trackNumber );

    private:
      class CDInfoPrivate * const d;
  };

  typedef QList<CDInfo> CDInfoList;
}

#endif // KCDDB_CDINFO_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
