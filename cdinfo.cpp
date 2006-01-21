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

#include <kdebug.h>
#include <kstringhandler.h>

#include "cdinfo.h"
#include <client.h>
#include <qmap.h>

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
        return data[type.upper()];
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
        //kdDebug() << "set: " << type << ", " << d.toString() << endl;
        if(type.find("^T.*_.*$" ) != -1){
          kdDebug(60010) << "Error: custom cdinfo::set data can not start with T and contain a _" << endl;
          return;
        }
        if(type.upper() == "DTITLE"){
          kdDebug(60010) << "Error: type: DTITLE is reserved and can not be set." << endl;
          return;
        }
        
        data[type.upper()] = d;
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
      append(const QString type, const QString& text)
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
      kdDebug(60010) << "Warning toString() on a track that doesn't have track number assigned." << endl;
    QMap<QString, QVariant>::const_iterator i = d->data.constBegin();
    while (i != d->data.constEnd()) {
        if(i.key() != "COMMENT" && i.key() != "TITLE" && i.key() != "ARTIST" && i.key() != "TRACKNUMBER") {
          out += QString("T%1_%2=%3\n").arg(i.key()).arg(track).arg( i.data().toString());
        }
        ++i;
    }
    return out;
  }

  
  class CDInfoPrivate : public InfoBasePrivate {
  };
  
  CDInfo::CDInfo()
  {
    d = new CDInfoPrivate();
    set("revision", 0);
  }

  CDInfo::CDInfo(const CDInfo& clone)
    : trackInfoList(clone.trackInfoList)
  {
    d = new CDInfoPrivate();
    d->data = clone.d->data;
  }

  CDInfo::~CDInfo()
  {
    delete d;
  }

  CDInfo& CDInfo::operator=(const CDInfo& clone)
  {
    trackInfoList = clone.trackInfoList;
    d->data = clone.d->data;
    return *this;
  }

    bool
  CDInfo::load(const QString & string)
  {
    return load(QStringList::split('\n', string));
  }

    bool
  CDInfo::load(const QStringList & lineList)
  {
    clear();

    // We'll append to this until we've seen all the lines, then parse it after.
    QString dtitle;

    QStringList::ConstIterator it = lineList.begin();

    QRegExp rev("# Revision: (\\d+)");

    while ( it != lineList.end() )
    {
      QString line(*it);
      ++it;

      QStringList tokenList = KStringHandler::perlSplit('=', line, 2);

      if (rev.search(line) != -1)
      {
        set("revision", rev.cap(1).toUInt());
        continue;
      }

      QString key = tokenList[0].stripWhiteSpace();
      QString value;
      if (2 != tokenList.count())
      {
        if (!key.startsWith("EXT"))
          continue;
      }
      else
        value = d->unescape ( tokenList[1].stripWhiteSpace() );

      if ( "DTITLE" == key )
      {
        dtitle += value;
      }
      else if ( "TTITLE" == key.left( 6 ) )
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
      else if ( "EXTT" == key.left( 4 ) )
      {
        uint trackNumber = key.mid( 4 ).toUInt();

        checkTrack( trackNumber );

        QString extt = track(trackNumber).get(Comment).toString();
        track(trackNumber).set(Comment, extt+value );
      
      }
      else if ( "T" == key.left( 1 ))
      {
        // Custom Track data
        uint trackNumber = key.mid( key.find('_')+1 ).toUInt();
        checkTrack( trackNumber );
        
        QString data = QString("^T.*_%1$").arg(trackNumber);
        if  ( key.find( data ) )
        {
          QString k = key.mid(1, key.find('_')-1);
          TrackInfo& ti = track(trackNumber);
          ti.set( k, ti.get(k).toString().append(value) );
        }
      }
      else if (key.find("^.*$" ))
      {
        // Custom Disk data
        d->append( key, value );
      }
    }

    int slashPos = dtitle.find('/');

    if (-1 == slashPos)
    {
      // Use string for title _and_ artist.
      set(Artist, dtitle);
      set(Title, dtitle);
    }
    else
    {
      set(Artist, dtitle.left(slashPos).stripWhiteSpace());
      set(Title, dtitle.mid(slashPos + 1).stripWhiteSpace());
    }

    bool isSampler = true;
    for (TrackInfoList::Iterator it = trackInfoList.begin(); it != trackInfoList.end(); ++it)
    {
      if ((*it).get(Title).toString().find(" / ") == -1)
      {
        isSampler = false;
      }
    }
    for (TrackInfoList::Iterator it = trackInfoList.begin(); it != trackInfoList.end(); ++it)
    {
      if (isSampler)
      {
        int delimiter = (*it).get(Title).toString().find(" / ");
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

    kdDebug(60010) << "Loaded CDInfo for " << get("discid").toString() << endl;

    return true;
  }

    QString
  CDInfo::toString(bool submit) const
  {
    QString s;

    if (get("revision") != 0)
      s += "# Revision: " + get("revision").toString() + "\n";

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
    s += "DYEAR=" + (0 == year ? QString::null : QString::number(year)) + "\n";
    if (get(Genre) == "Unknown")
      s += d->createLine("DGENRE", "");
    else
      s += d->createLine("DGENRE",get(Genre).toString());
    
    for (int i = 0; i < trackInfoList.count(); ++i){
      QString trackTitle = trackInfoList[i].get(Title).toString();
      QString trackArtist = trackInfoList[i].get(Artist).toString();
      if(trackArtist != artist && !trackArtist.isEmpty())
        s += d->createLine(QString("TTITLE%1").arg(i), QString("%1 / %2").arg(trackArtist).arg(trackTitle));
      else
        s += d->createLine(QString("TTITLE%1").arg(i), trackTitle);
    }
    
    s += d->createLine(QString("EXTD"), get(Comment).toString());
    
    for (int i = 0; i < trackInfoList.count(); ++i)
      s += d->createLine(QString("EXTT%1").arg(i), trackInfoList[i].get(Comment).toString());
    
    if (submit)
    {
      s += d->createLine("PLAYORDER", "");
      return s;
    }

    s += d->createLine("PLAYORDER", get("playorder").toString() );
    
    // Custom track data
    for (int i = 0; i < trackInfoList.count(); ++i)
    {
      QStringList lines = QStringList::split('\n', trackInfoList[i].toString(), true);
      for (QStringList::iterator it = lines.begin(); it != lines.end(); ++it)
        if(!(*it).isEmpty())
          s += d->createLine((*it).mid(0,(*it).find('=')), (*it).mid((*it).find('=')+1) );
    }

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
      if (!cddbKeywords.contains(i.key()))
      {
        s+= d->createLine(QString("%1").arg(i.key()), i.data().toString());
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
    while ( trackInfoList.count() < trackNumber + 1 ){
      int count = trackInfoList.count();
      trackInfoList.append(TrackInfo());
      trackInfoList[count].set("tracknumber", count);
    }
  }

    void
  CDInfo::clear()
  {
    d->data.clear();
    trackInfoList.clear();
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
    return trackInfoList[trackNumber];
  }
    
    TrackInfo
  CDInfo::track( int trackNumber ) const
  {
    if (trackInfoList.count() < trackNumber)
      return trackInfoList[trackNumber];
    else
      return TrackInfo();
  }
    
    int
  CDInfo::numberOfTracks() const
  {
    return trackInfoList.count();
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
