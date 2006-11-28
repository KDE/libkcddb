/*
    This file is part of libkcddb.
    Copyright (c) 2006 Laurent Montel <montel@kde.org>

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

#ifndef KCDDB_EXPORT_H
#define KCDDB_EXPORT_H

/* needed for KDE_EXPORT macros */
#include <kdemacros.h>

#if defined Q_OS_WIN
#ifndef KCDDB_EXPORT
# ifdef MAKE_KCDDB_LIB
#  define KCDDB_EXPORT KDE_EXPORT
# else
#  define KCDDB_EXPORT KDE_IMPORT
# endif
#endif

#else /* UNIX*/

/* export statements for unix */
#define KCDDB_EXPORT KDE_EXPORT

#endif

#endif
