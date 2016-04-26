/*
  Copyright (C) 2007 Richard Lärkäng <nouseforaname@home.se>
  Copyright (C) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>

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

#ifndef KCDDB_H
#define KCDDB_H

#include <QtCore/QList>
#include <QtCore/QString>

#include <libkcddb/kcddb_export.h>

namespace KCDDB
{
  /** This list is used to calculate the CDDB disc id.
    Insert the start frames ((minute*60 + seconds)*75+frames)
    of all tracks, followed by the last frame of the disc. The
    last frame is the start frame of the leadout track.
    */
  typedef QList<uint> TrackOffsetList;

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

  KCDDB_EXPORT QString resultToString(Result);
}

#endif
