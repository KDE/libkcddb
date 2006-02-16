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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include <kdebug.h>
#include <kstringhandler.h>

#include "cdinfo.h"
#include "cddb.h"

namespace KCDDB
{
  TrackInfo::TrackInfo()
  {
  }

  TrackInfo::TrackInfo(const TrackInfo& clone)
      : title(clone.title),
        extt(clone.extt)
  {
  }

  TrackInfo::~TrackInfo()
  {
  }

  TrackInfo& TrackInfo::operator=(const TrackInfo& clone)
  {
    title = clone.title;
    extt = clone.extt;
    return *this;
  }


  CDInfo::CDInfo()
    : year(0),
      length(0),
      revision(0)
  {
  }

  CDInfo::CDInfo(const CDInfo& clone)
    : id(clone.id),
      artist(clone.artist),
      title(clone.title),
      genre(clone.genre),
      category(clone.category),
      extd(clone.extd),
      year(clone.year),
      length(clone.length),
      revision(clone.revision),
      trackInfoList(clone.trackInfoList)

  {
  }

  CDInfo::~CDInfo()
  {
  }

  CDInfo& CDInfo::operator=(const CDInfo& clone)
  {
    id = clone.id;
    artist = clone.artist;
    title = clone.title;
    genre = clone.genre;
    category = clone.category;
    extd = clone.extd;
    year = clone.year;
    length = clone.length;
    revision = clone.revision;
    trackInfoList = clone.trackInfoList;
    return *this;
  }

  QVariant TrackInfo::get(const QString &type) const {
    if(type == "title")
      return title;
    if(type == "extt")
      return extt;
    return QVariant();
  }
  
    bool
  CDInfo::load(const QString & s)
  {
    return load(QStringList::split('\n', s));
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

      if ( "DISCID" == key )
      {
        id = value;
      }
      else if ( "DTITLE" == key )
      {
        dtitle += value;
      }
      else if ( "DYEAR" == key )
      {
        year = value.toUInt();
      }
      else if ( "DGENRE" == key )
      {
        genre += value;
      }
      else if ( "TTITLE" == key.left( 6 ) )
      {
        uint trackNumber = key.mid(6).toUInt();

        checkTrack( trackNumber );

        trackInfoList[ trackNumber ].title.append( value );
      }
      else if ( "EXTD" == key )
      {
        extd.append( value );
      }
      else if ( "EXTT" == key.left( 4 ) )
      {
        uint trackNumber = key.mid( 4 ).toUInt();

        checkTrack( trackNumber );

        trackInfoList[ trackNumber ].extt.append( value );
      }
    }

    int slashPos = dtitle.find('/');

    if (-1 == slashPos)
    {
      // Use string for title _and_ artist.
      artist = title = dtitle;
    }
    else
    {
      artist  = dtitle.left(slashPos).stripWhiteSpace();
      title   = dtitle.mid(slashPos + 1).stripWhiteSpace();
    }

    if ( genre.isEmpty() )
      genre = "Unknown";

    kdDebug(60010) << "Loaded CDInfo for " << id << endl;

    return true;
  }

    QString
  CDInfo::toString(bool submit) const
  {
    QString s;

    if (revision != 0)
      s += "# Revision: " + QString::number(revision) + "\n";

    if (submit)
    {
      s += "#\n";
      s += QString("# Submitted via: %1 %2\n").arg(CDDB::clientName(),
        CDDB::clientVersion());
    }

    s += "DISCID=" + escape( id ) + "\n";
    s += createLine("DTITLE",escape( artist ) + " / " + escape( title ));
    s += "DYEAR=" + (0 == year ? QString::null : QString::number(year)) + "\n";
    s += createLine("DGENRE",escape( genre ));

    for (uint i = 0; i < trackInfoList.count(); ++i)
    {
      s += createLine(QString("TTITLE%1").arg(i), 
                escape( trackInfoList[ i ].title));
    }

    s += createLine("EXTD", escape( extd ));

    for (uint i = 0; i < trackInfoList.count(); ++i)
    {
      s += createLine(QString("EXTT%1").arg(i), escape(trackInfoList[i].extt));
    }

    s +="PLAYORDER=\n";

    return s;
  }

  // Creates a line in the form NAME=VALUE, and splits it into several
  // lines if the line gets longer than 256 chars
    QString
  CDInfo::createLine(const QString& name, const QString& value) const
  {
    Q_ASSERT(name.length() < 254);

    uint maxLength = 256 - name.length() - 2;

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
  CDInfo::checkTrack( uint trackNumber )
  {
    if ( trackInfoList.count() < trackNumber + 1 )
    {
      while ( trackInfoList.count() < trackNumber + 1 )
        trackInfoList.append(TrackInfo());
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
    id = artist = title = genre = extd = QString::null;
    length = year = revision = 0;
    trackInfoList.clear();
  }

    bool
  CDInfo::isValid() const
  {
    if (id.isEmpty())
      return false;

    if (id == "0")
      return false;

    return true;
  }

  QVariant CDInfo::get(const QString &type) const {
    if(type == "id")
      return id;
    if(type == "artist")
      return artist;
    if(type == "title")
      return title;
    if(type == "genre")
      return genre;
    if(type == "category")
      return category;
    if(type == "extd")
      return extd;
    if(type == "year")
      return year;
    if(type == "length")
      return length;
    if(type == "revision")
      return revision;
    return QVariant();
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
