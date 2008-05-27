/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002-2005 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2002-2004 Nadeem Hasan <nhasan@nadmm.com>
  Copyright (C) 2006 Richard Lärkäng <nouseforaname@home.se>

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

#include "cdinfo.h"

#include "client.h"
#include "cddb.h"

#include <kdebug.h>
#include <kstringhandler.h>

#include <QMap>

namespace KCDDB
{
  class InfoBasePrivate {
    public:
      /**
       * Creates a line in the form NAME=VALUE, and splits it into several
       * lines if the line gets longer than 256 chars
       */
        static QString
      createLine(const QString& name, const QString& value)
      {
        Q_ASSERT(name.length() < 254);

        int maxLength = 256 - name.length() - 2;

        QString tmpValue = escape(value);

        QString lines;

        while (tmpValue.length() > maxLength)
        {
          lines += QString("%1=%2\n").arg(name,tmpValue.left(maxLength));
          tmpValue = tmpValue.mid(maxLength);
        }

        lines += QString("%1=%2\n").arg(name,tmpValue);

        return lines;
      }

      /**
       * escape's string for CDDB processing
       */
        static QString
      escape( const QString &value )
      {
        QString s = value;
        s.replace( "\\", "\\\\" );
        s.replace( "\n", "\\n" );
        s.replace( "\t", "\\t" );

        return s;
      }

      /**
       * fixes an escaped string that has been CDDB processed
       */
        static QString
      unescape( const QString &value )
      {
        QString s = value;

        s.replace( "\\n", "\n" );
        s.replace( "\\t", "\t" );
        s.replace( "\\\\", "\\" );

        return s;
      }

        QVariant
      get(const QString& type)
      {
        return data[type.toUpper()];
      }
        QVariant
      get(Type type)
      {
        switch(type){
          case(Title):
            return get("title");
          case(Comment):
            return get("comment");
          case(Artist):
            return get("artist");
          case(Genre):
            return get("genre");
          case(Year):
            return get("year");
          case(Length):
            return get("length");
          case(Category):
            return get("category");
        }
        return QVariant();
      }

        void
      set(const QString& type, const QVariant &d)
      {
        //kDebug() << "set: " << type << ", " << d.toString();
        if(type.contains(QRegExp( "^T.*_.*$" ) )){
          kDebug(60010) << "Error: custom cdinfo::set data can not start with T and contain a _";
          return;
        }
        if(type.toUpper() == "DTITLE"){
          kDebug(60010) << "Error: type: DTITLE is reserved and can not be set.";
          return;
        }

        data[type.toUpper()] = d;
      }
        void
      set(Type type, const QVariant &d)
      {
        switch(type)
        {
          case(Title):
            set("title", d);
            return;
          case(Comment):
            set("comment", d);
            return;
          case(Artist):
            set("artist", d);
            return;
          case(Genre):
            set("genre", d);
            return;
          case(Year):
            set("year", d);
            return;
          case(Length):
            set("length", d);
            return;
          case(Category):
            set("category", d);
            return;
        }

        Q_ASSERT(false);
      }

      // Appends text to data instead of overwriting it
        void
      append(const QString& type, const QString& text)
      {
        set(type, get(type).toString().append(text));
      }
        void
      append(Type type, const QString& text)
      {
        set(type, get(type).toString().append(text));
      }

      QMap<QString, QVariant> data;
  } ;

  class TrackInfoPrivate : public InfoBasePrivate {
  };

  TrackInfo::TrackInfo()
  {
    d = new TrackInfoPrivate();
  }

  TrackInfo::TrackInfo(const TrackInfo& clone)
      : d(new TrackInfoPrivate)
  {
    d->data = clone.d->data;
  }

  TrackInfo::~TrackInfo()
  {
     delete d;
  }

  TrackInfo& TrackInfo::operator=(const TrackInfo& clone)
  {
    d->data = clone.d->data;
    return *this;
  }

  QVariant TrackInfo::get(Type type) const {
    return d->get(type);
  }

  QVariant TrackInfo::get(const QString &type) const {
    return d->get(type);
  }

  void TrackInfo::set(const QString &type, const QVariant &data){
    d->set(type, data);
  }

  void TrackInfo::set(Type type, const QVariant &data) {
    d->set(type, data);
  }

  void TrackInfo::clear(){
    d->data.clear();
  }

  QString TrackInfo::toString() const {
    QString out;
    bool ok;
    int track = get("tracknumber").toInt(&ok);
    if(!ok)
      kDebug(60010) << "Warning toString() on a track that doesn't have track number assigned.";
    QMap<QString, QVariant>::const_iterator i = d->data.constBegin();
    while (i != d->data.constEnd()) {
        if(i.key() != "COMMENT" && i.key() != "TITLE" && i.key() != "ARTIST" && i.key() != "TRACKNUMBER") {
          out += d->createLine(QString("T%1_%2").arg(i.key()).arg(track),i.value().toString());
        }
        ++i;
    }
    return out;
  }

    bool TrackInfo::operator==( const TrackInfo& other ) const
    {
        return d->data == other.d->data;
    }

    bool TrackInfo::operator!=( const TrackInfo& other ) const
    {
        return d->data != other.d->data;
    }

  class CDInfoPrivate : public InfoBasePrivate {
    public:
      TrackInfoList trackInfoList;
  };

  CDInfo::CDInfo()
    : d(new CDInfoPrivate())
  {
    set("revision", 0);
  }

  CDInfo::CDInfo(const CDInfo& clone)
    : d(new CDInfoPrivate())
  {
    d->data = clone.d->data;
    d->trackInfoList = clone.d->trackInfoList;
  }

  CDInfo::~CDInfo()
  {
    delete d;
  }

  CDInfo& CDInfo::operator=(const CDInfo& clone)
  {
    d->trackInfoList = clone.d->trackInfoList;
    d->data = clone.d->data;
    return *this;
  }

    bool
  CDInfo::load(const QString & string)
  {
    return load(string.split('\n',QString::SkipEmptyParts));
  }

    bool
  CDInfo::load(const QStringList & lineList)
  {
    clear();

    // We'll append to this until we've seen all the lines, then parse it after.
    QString dtitle;

    QStringList::ConstIterator it = lineList.begin();

    QRegExp rev("# Revision: (\\d+)");
    QRegExp eol("[\r\n]");

    while ( it != lineList.end() )
    {
      QString line(*it);
      line.replace(eol,"");
      ++it;

      if (rev.indexIn(line) != -1)
      {
        set("revision", rev.cap(1).toUInt());
        continue;
      }

      QStringList tokenList = KStringHandler::perlSplit('=', line, 2);

      if (2 != tokenList.count())
      {
        continue;
      }

      QString key = tokenList[0].trimmed();
      QString value = d->unescape ( tokenList[1] );

      if ( "DTITLE" == key )
      {
        dtitle += value;
      }
      else if ( key.startsWith("TTITLE") )
      {
        uint trackNumber = key.mid(6).toUInt();

        TrackInfo& ti = track(trackNumber);
        ti.set(Title, ti.get(Title).toString().append(value));
      }

      else if ( "EXTD" == key )
      {
        d->append(Comment, value);
      }
      else if ( "DGENRE" == key )
      {
        d->append(Genre, value);
      }
      else if ( "DYEAR" == key )
      {
        set(Year, value);
      }
      else if ( key.startsWith("EXTT") )
      {
        uint trackNumber = key.mid( 4 ).toUInt();

        checkTrack( trackNumber );

        QString extt = track(trackNumber).get(Comment).toString();
        track(trackNumber).set(Comment, extt+value );
      }
      else if ( key.startsWith("T") )
      {
        // Custom Track data
        uint trackNumber = key.mid( key.indexOf('_')+1 ).toUInt();
        checkTrack( trackNumber );

        QRegExp data(QString("^T.*_%1$").arg(trackNumber));
        if  ( key.contains( data ) )
        {
          QString k = key.mid(1, key.indexOf('_')-1);
          TrackInfo& ti = track(trackNumber);
          ti.set( k, ti.get(k).toString().append(value) );
        }
      }
      else
      {
        // Custom Disk data
        d->append( key, value );
      }
    }

    int slashPos = dtitle.indexOf(" / ");

    if (-1 == slashPos)
    {
      // Use string for title _and_ artist.
      set(Artist, dtitle);
      set(Title, dtitle);
    }
    else
    {
      set(Artist, dtitle.left(slashPos).trimmed());
      set(Title, dtitle.mid(slashPos + 3).trimmed());
    }

    bool isSampler = true;
    for (TrackInfoList::Iterator it = d->trackInfoList.begin(); it != d->trackInfoList.end(); ++it)
    {
      if (!(*it).get(Title).toString().contains(" / "))
      {
        isSampler = false;
      }
    }
    for (TrackInfoList::Iterator it = d->trackInfoList.begin(); it != d->trackInfoList.end(); ++it)
    {
      if (isSampler)
      {
        int delimiter = (*it).get(Title).toString().indexOf(" / ");
        (*it).set(Artist, (*it).get(Title).toString().left(delimiter));
        (*it).set(Title, (*it).get(Title).toString().mid(delimiter + 3));
      }
      else
      {
        (*it).set(Artist, get(Artist));
      }
    }

    if ( get(Genre).toString().isEmpty() )
      set(Genre, "Unknown");

    kDebug(60010) << "Loaded CDInfo for " << get("discid").toString();

    return true;
  }

    QString
  CDInfo::toString(bool submit) const
  {
    QString s;

    if (get("revision") != 0)
      s += "# Revision: " + get("revision").toString() + '\n';

    // If we are submiting make it a fully compliant CDDB entry
    if (submit)
    {
      s += "#\n";
      s += QString("# Submitted via: %1 %2\n").arg(CDDB::clientName(),
        CDDB::clientVersion());
    }

    s += d->createLine("DISCID", get("discid").toString() );
    QString artist = get(Artist).toString();
    s += d->createLine("DTITLE", artist + " / " + get(Title).toString() );
    int year = get(Year).toInt();
    s += "DYEAR=" + (0 == year ? QString::null : QString::number(year)) + '\n';	//krazy:exclude=nullstrassign for old broken gcc
    if (get(Genre) == "Unknown")
      s += d->createLine("DGENRE", "");
    else
      s += d->createLine("DGENRE",get(Genre).toString());

    for (int i = 0; i < d->trackInfoList.count(); ++i){
      QString trackTitle = d->trackInfoList[i].get(Title).toString();
      QString trackArtist = d->trackInfoList[i].get(Artist).toString();
      if(trackArtist != artist && !trackArtist.isEmpty())
        s += d->createLine(QString("TTITLE%1").arg(i), QString("%1 / %2").arg(trackArtist).arg(trackTitle));
      else
        s += d->createLine(QString("TTITLE%1").arg(i), trackTitle);
    }

    s += d->createLine(QString("EXTD"), get(Comment).toString());

    for (int i = 0; i < d->trackInfoList.count(); ++i)
      s += d->createLine(QString("EXTT%1").arg(i), d->trackInfoList[i].get(Comment).toString());

    if (submit)
    {
      s += d->createLine("PLAYORDER", "");
      return s;
    }

    s += d->createLine("PLAYORDER", get("playorder").toString() );

    // Custom track data
    for (int i = 0; i < d->trackInfoList.count(); ++i)
      s += d->trackInfoList[i].toString();

    QStringList cddbKeywords;
    cddbKeywords
      << "DISCID"
      << "ARTIST"
      << "TITLE"
      << "COMMENT"
      << "YEAR"
      << "GENRE"
      << "PLAYORDER"
      << "CATEGORY"
      << "REVISION";

    // Custom disc data
    QMap<QString, QVariant>::const_iterator i = d->data.constBegin();
    while (i != d->data.constEnd()){
      if (!cddbKeywords.contains(i.key()) && i.key() != "SOURCE")
      {
        s+= d->createLine(i.key(), i.value().toString());
      }
      ++i;
    }

    return s;
  }

  QVariant CDInfo::get(Type type) const {
    return d->get(type);
  }

  QVariant CDInfo::get(const QString &type) const {
    return d->get(type);
  }

  void CDInfo::set(const QString &type, const QVariant &data){
    d->set(type, data);
  }

  void CDInfo::set(Type type, const QVariant &data) {
    d->set(type, data);
  }


    void
  CDInfo::checkTrack( int trackNumber )
  {
    while ( d->trackInfoList.count() <= trackNumber ){
      int count = d->trackInfoList.count();
      d->trackInfoList.append(TrackInfo());
      d->trackInfoList[count].set("tracknumber", count);
    }
  }

    void
  CDInfo::clear()
  {
    d->data.clear();
    d->trackInfoList.clear();
  }

    bool
  CDInfo::isValid() const
  {
    QString discid = get("DISCID").toString();
    if (discid.isEmpty())
      return false;

    if (discid == "0")
      return false;

    return true;
  }

    TrackInfo &
  CDInfo::track( int trackNumber )
  {
    checkTrack( trackNumber );
    return d->trackInfoList[trackNumber];
  }

    TrackInfo
  CDInfo::track( int trackNumber ) const
  {
    if (trackNumber < d->trackInfoList.count())
      return d->trackInfoList[trackNumber];
    else
    {
      kWarning() << "Couldn't find track " << trackNumber;
      return TrackInfo();
    }
  }

    int
  CDInfo::numberOfTracks() const
  {
    return d->trackInfoList.count();
  }

    bool CDInfo::operator==( const CDInfo& other ) const
    {
        return(  d->data == other.d->data &&
                 d->trackInfoList == other.d->trackInfoList );
    }

    bool CDInfo::operator!=( const CDInfo& other ) const
    {
        return(  d->data != other.d->data ||
                 d->trackInfoList != other.d->trackInfoList );
    }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
