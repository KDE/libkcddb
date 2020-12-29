/*
    SPDX-FileCopyrightText: 2005 Shaheed Haque <srhaque@iee.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KCDDB_GENRES_H
#define KCDDB_GENRES_H

#include <kcddb_export.h>
#include <QStringList>

namespace KCDDB
{
    /**
     * The Genre is completely arbitrary. But we follow kaudiocreator's cue
     * and make life easy for people.
     */
    class KCDDB_EXPORT Genres
    {
    public:
        Genres();
        Genres(const Genres&);
        ~Genres();

        Genres& operator=(const Genres&);

        const QStringList &cddbList() const;
        const QStringList &i18nList() const;

        /**
         * Lookup the CDDB genre, and return the i18n'd version.
         */
        const QString cddb2i18n(const QString &genre) const;

        /**
         * Lookup the i18n genre, and return the CDDB version if we can.
         */
        const QString i18n2cddb(const QString &genre) const;
    private:
        class Private;
        Private * const d;
    };
}

#endif
