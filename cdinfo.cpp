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

#include <kdebug.h>
#include <kstringhandler.h>
#include "cdinfo.h"

namespace KCDDB
{
  CDInfo::CDInfo()
    : year(0),
      length(0)
  {
  }

    bool
  CDInfo::load(const QString & s)
  {
    return load(QStringList::split('\n', s));
  }

    bool
  CDInfo::load(const QStringList & lineList)
  {
    QStringList::ConstIterator it;
      
    for (it = lineList.begin(); it != lineList.end(); ++it)
    {
      QString line(*it);

      QStringList tokenList = KStringHandler::perlSplit('=', line, 2);

      if (2 != tokenList.count())
        continue;

      QString key   = tokenList[0];
      QString value = tokenList[1];

      kdDebug() << "Useful line. Key == `" << key << "'" << endl;

      value.replace(QRegExp("\\n"), "\n");
      value.replace(QRegExp("\\t"), "\t");
      value.replace(QRegExp("\\\\"), "\\");

      if ('D' == key[0])
      {
        if ("DTITLE" == key)
        {
          int slashPos = value.find('/');

          if (-1 == slashPos)
          {
            // Use string for title _and_ artist.
            artist = title = value.stripWhiteSpace();
          }
          else
          {
            artist  = value.left(slashPos).stripWhiteSpace();
            title   = value.mid(slashPos + 1).stripWhiteSpace();
          }
        }
        else if ("DYEAR" == key)
        {
          year = value.toUInt();
        }
        else if ("DGENRE" == key)
        {
          genre = value;
        }
      }
      else if ("TTITLE" == key.left(6))
      {
        uint trackNumber = key.mid(6).toUInt();

        if (trackNumber > 200)
        {
          kdDebug() << "Track number out of sensible range." << endl;
          continue;
        }

        TrackInfo trackInfo;
        trackInfo.title = value.stripWhiteSpace();

        while (trackInfoList.size() < trackNumber + 1)
        {
          trackInfoList.append(TrackInfo());
        }

        trackInfoList[trackNumber] = trackInfo;
      }
    }

    return true;
  }

    QString
  CDInfo::toString() const
  {
    QString s;

    s += "DISCID=" + id + "\n";
    s += "DTITLE=" + artist + "/" + title + "\n";
    s += "DYEAR=" + QString::number(year) + "\n";
    s += "DGENRE=" + genre + "\n";

    TrackInfoList::ConstIterator it(trackInfoList.begin());

    for (uint i = 0; it != trackInfoList.end(); ++it, ++i)
    {
      s += "TTITLE" + QString::number(i) + "=" + (*it).title + "\n";
    }

    return s;
  }
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
