/*
    SPDX-FileCopyrightText: 2007 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCDDB_H
#define KCDDB_H

#include <QList>
#include <QString>

#include <kcddb_export.h>

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
