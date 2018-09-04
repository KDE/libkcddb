// Copyright (C) 2005 by Shaheed Haque (srhaque@iee.org). All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
#ifndef KCDDB_CATEGORIES_H
#define KCDDB_CATEGORIES_H

#include <kcddb_export.h>
#include <QString>
#include <QStringList>

namespace KCDDB
{
   /**
    * Category values defined by CDDB.
    */
    class KCDDB_EXPORT Categories
    {
    public:
        Categories();
        Categories(const Categories&);
        ~Categories();

        Categories& operator=(const Categories&);

        const QStringList &cddbList() const;
        const QStringList &i18nList() const;
        
        /**
         * Lookup the CDDB category, and return the i18n'd version.
         */
        const QString cddb2i18n(const QString &category) const;

        /**
         * Lookup the i18n category, and return the CDDB version.
         */
        const QString i18n2cddb(const QString &category) const;
    private:
        class Private;
        Private * const d;
    };
}

#endif
