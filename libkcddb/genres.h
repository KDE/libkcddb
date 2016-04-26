// Copyright (C) 2005 by Shaheed Haque (srhaque@iee.org). All rights reserved.
// Copyright (C) 2016 Angelo Scarnà <angelo.scarna@codelinsoft.it>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
#ifndef KCDDB_GENRES_H
#define KCDDB_GENRES_H

#include <QStringList>
#include <libkcddb/kcddb_export.h>

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
