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
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include <kdebug.h>
#include <kstringhandler.h>

#include "cdinfo.h"
#include <client.h>
#include <qmap.h>

namespace KCDDB
{
  class TrackInfoPrivate {
    public:
    TrackInfoPrivate(){
    };
    QMap<QString, QVariant> data;
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
    switch(type){
      case(Title):
        return get("title");
      case(Extt):
        return get("extt");
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

  QVariant TrackInfo::get(const QString &type) const {
    return d->data[type.upper()];
  }
  
  void TrackInfo::set(const QString &type, const QVariant &data){
    d->data[type.upper()] = data;
  }
 
  void TrackInfo::clear(){
    d->data.clear();
  }
  
  QString TrackInfo::toString() const {
    QString out;
    int track = get("tracknumber").toInt();
    if(track <= 0)
      kdDebug(60010) << "Warning toString() on a track that doesn't have track number assigned." << endl;
    QMap<QString, QVariant>::const_iterator i = d->data.constBegin();
    while (i != d->data.constEnd()) {
        QString extra;
        if(i.key() == "EXTT") 
          out += QString("%1%2=%4\n").arg(i.key()).arg(track).arg( i.data().toString());
        else {
          if (i.key() != "TITLE") 
            extra = "_";
          if (i.key() != "TRACKNUMBER")
            out += QString("T%1%2%3=%4\n").arg(i.key()).arg(extra).arg(track).arg( i.data().toString());
        }
        ++i;
    }
    return out;
  }

  
  class CDInfoPrivate {
    public:
    CDInfoPrivate(){
    };
    QMap<QString, QVariant> data;
  };
  
  CDInfo::CDInfo()
    : revision(0)
  {
    d = new CDInfoPrivate();
  }

  CDInfo::CDInfo(const CDInfo& clone)
    : revision(clone.revision),
      trackInfoList(clone.trackInfoList)

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
    revision = clone.revision;
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

    for (int i = 0; i < trackInfoList.count(); ++i){
      trackInfoList[i].clear();
      trackInfoList[i].set("tracknumber", i);
    }
    
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
        revision = rev.cap(1).toUInt();;
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
        value = unescape ( tokenList[1].stripWhiteSpace() );

      if ( "DTITLE" == key )
      {
        dtitle += value;
      }
      else if ( "TTITLE" == key.left( 6 ) )
      {
        uint trackNumber = key.mid(6).toUInt();

        checkTrack( trackNumber );

        trackInfoList[ trackNumber ].set("title", value );
      }
      
      else if ( "EXTD" == key )
      {
        QString extd = get("extd").toString();
        if (!extd.isEmpty())
          extd.append('\n');
        extd.append( value );
        set("extd", extd);
      }
      else if ( "EXTT" == key.left( 4 ) )
      {
        uint trackNumber = key.mid( 4 ).toUInt();

        checkTrack( trackNumber );

        QString extt = trackInfoList[ trackNumber ].get("extt").toString();
        if (!(extt).isEmpty())
          extt.append('\n');
        trackInfoList[ trackNumber ].set("extt", extt+value );
      
      }
      else if ( "T" == key.left( 1 ))
      {
        // Custom Track data
        uint trackNumber = key.mid( key.find('_')+1 ).toUInt();
        checkTrack( trackNumber );
        
        QString data = QString("^T.*_%1$").arg(trackNumber);
        if  ( key.find( data ) )
          trackInfoList[ trackNumber ].set( key.mid(1, key.find('_')-1), value);
      }
      else if (key.find("^.*$" ))
      {
        // Custom Disk data
        set( key, value);
      }
    }

    int slashPos = dtitle.find('/');

    if (-1 == slashPos)
    {
      // Use string for title _and_ artist.
      set("artist", dtitle);
      set("title", dtitle);
    }
    else
    {
      set("artist", dtitle.left(slashPos).stripWhiteSpace());
      set("title", dtitle.mid(slashPos + 1).stripWhiteSpace());
    }

    if ( get("genre").toString().isEmpty() )
      set("genre", "Unknown");

    kdDebug(60010) << "Loaded CDInfo for " << get("discid").toString() << endl;

    return true;
  }

    QString
  CDInfo::toString(bool submit) const
  {
    QString s;

    if (revision != 0)
      s += "# Revision: " + QString::number(revision) + "\n";

    // If we are submiting make it a fully compliant CDDB entry
    if (submit)
    {
      s += "#\n";
      s += QString("# Submitted via: %1 %2\n").arg(CDDB::clientName(),
        CDDB::clientVersion());

      s += "DISCID=" + escape( get("discid").toString() ) + "\n";
      QString artist = get("artist").toString();
      s += createLine("DTITLE",escape( artist ) + " / " + escape( get("title").toString() ));
      int year = get("year").toInt();
      s += "DYEAR=" + (0 == year ? QString::null : QString::number(year)) + "\n";
      s += createLine("DGENRE",escape( get("genre").toString() ));
      
      for (int i = 0; i < trackInfoList.count(); ++i){
        QString trackTitle = trackInfoList[i].get("title").toString();
        QString trackArtist = trackInfoList[i].get("artist").toString();
        if(trackArtist != artist && !trackArtist.isEmpty())
          s += createLine(QString("TTITLE%1").arg(i), QString("%1 / %2").arg(trackArtist).arg(trackTitle));
        else
          s += createLine(QString("TTITLE%1").arg(i), trackTitle);
      }
      
      s += createLine(QString("EXTD"), escape(get("extd").toString()));
      
      for (int i = 0; i < trackInfoList.count(); ++i)
        s += createLine(QString("EXTT%1").arg(i), trackInfoList[i].get("extt").toString());
      
      s += createLine("PLAYORDER", escape( get("playorder").toString() ));

      return s;
    }
    
    // Custom track data
    for (int i = 0; i < trackInfoList.count(); ++i)
    {
      QStringList lines = QStringList::split('\n', trackInfoList[i].toString(), true);
      for (QStringList::iterator it = lines.begin(); it != lines.end(); ++it)
        if(!(*it).isEmpty())
          s += createLine((*it).mid(0,(*it).find('=')), escape( (*it).mid((*it).find('=')+1) ) );
    }

    // Custom disc data
    QMap<QString, QVariant>::const_iterator i = d->data.constBegin();
    while (i != d->data.constEnd()){ 
      s+= createLine(QString("%1").arg(i.key()), escape(i.data().toString()));  
      ++i;
    }
    
    return s;
  }

  QVariant CDInfo::get(const QString &type) const {
    // To make life easier
    if(type == "genre")
      return d->data["DGENRE"];
    if(type == "year")
      return d->data["DYEAR"];
    
    return d->data[type.upper()];
  }
  
  void CDInfo::set(const QString &type, const QVariant &data){
    //kdDebug() << "set: " << type << ", " << data.toString() << endl;
    if(type.find("^T.*_.*$" ) != -1){
      kdDebug(60010) << "Error: custom cdinfo::set data can not start with T and contain a _" << endl;
      return;
    }
    if(type.upper() == "DTITLE"){
      kdDebug(60010) << "Error: type: DTITLE is reserved and can not be set." << endl;
      return;
    }
    
    // To make life easier
    QString dtype = type;
    if(type == "year")
      dtype = "dyear";
    if(type == "genre")
      dtype = "dgenre";

    d->data[dtype.upper()] = data;
  }
    
  // Creates a line in the form NAME=VALUE, and splits it into several
  // lines if the line gets longer than 256 chars
    QString
  CDInfo::createLine(const QString& name, const QString& value) const
  {
    Q_ASSERT(name.length() < 254);

    int maxLength = 256 - name.length() - 2;

    QString tmpValue = value;

    QString lines;

    while (tmpValue.length() > maxLength)
    {
      lines += QString("%1=%2\n").arg(name,tmpValue.left(maxLength));
      tmpValue = tmpValue.mid(maxLength);
    }

    lines += QString("%1=%2\n").arg(name,tmpValue);

    return lines;
  }

    void
  CDInfo::checkTrack( int trackNumber )
  {
    if ( trackInfoList.count() < trackNumber + 1 )
    {
      while ( trackInfoList.count() < trackNumber + 1 ){
        int count = trackInfoList.count();
        trackInfoList.append(TrackInfo());
        trackInfoList[count].set("tracknumber", count);
      }
    }
  }

    QString
  CDInfo::escape( const QString& value )
  {
    QString s = value;
    s.replace( "\\", "\\\\" );
    s.replace( "\n", "\\n" );
    s.replace( "\t", "\\t" );

    return s;
  }

    QString
  CDInfo::unescape( const QString& value )
  {
    QString s = value;

    s.replace( "\\n", "\n" );
    s.replace( "\\t", "\t" );
    s.replace( "\\\\", "\\" );

    return s;
  }

    void
  CDInfo::clear()
  {
    revision = 0;
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
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
