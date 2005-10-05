/*
  Copyright (C) 2005 Richard Lärkäng <nouseforaname@home.se>

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

#ifndef KCDDB_CDINFOENCODINGWIDGET_H
#define KCDDB_CDINFOENCODINGWIDGET_H

#include "cdinfoencodingwidgetbase.h"

namespace KCDDB
{
    class CDInfoEncodingWidget : public QWidget, private Ui::CDInfoEncodingWidgetBase
    {
    Q_OBJECT
    public:
        CDInfoEncodingWidget(QWidget* parent, const QString& artist, const QString& title,
            const QStringList& songTitles);
        virtual ~CDInfoEncodingWidget() { }

        QString selectedEncoding();

    private slots:
        void slotEncodingChanged(const QString &);
        
    private:
        QString m_artist, m_title;
        QStringList m_songTitles;
    } ;
}

#endif // KCDDB_CDINFOENCODINGWIDGET_H
