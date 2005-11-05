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
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include <qcombobox.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qtextcodec.h>
#include <klocale.h>
#include <kglobal.h>
#include <kcharsets.h>
#include "cdinfoencodingwidget.h"

namespace KCDDB
{
  CDInfoEncodingWidget::CDInfoEncodingWidget(QWidget* parent, const QString& artist,
          const QString& title, const QStringList& songTitles)
    : CDInfoEncodingWidgetBase(parent), m_artist(artist), m_title(title),
        m_songTitles(songTitles)
  {
    encodingCombo->insertStringList(KGlobal::charsets()->descriptiveEncodingNames());

    slotEncodingChanged(encodingCombo->currentText());

    connect(encodingCombo,SIGNAL(activated(const QString&)),
        this,SLOT(slotEncodingChanged(const QString&)));
  }

  QString CDInfoEncodingWidget::selectedEncoding()
  {
    return encodingCombo->currentText();
  }

  void CDInfoEncodingWidget::slotEncodingChanged(const QString& encoding)
  {
    KCharsets* charsets = KGlobal::charsets();
    
    QTextCodec* codec = charsets->codecForName(charsets->encodingForName(encoding));
    
    songsBox->clear();
    QStringList newTitles;

    for (QStringList::const_iterator it = m_songTitles.begin();
        it != m_songTitles.end(); ++it)
      newTitles << codec->toUnicode((*it).latin1());

    songsBox->clear();
    songsBox->insertStringList(newTitles);
      
    titleLabel->setText(i18n("artist - cdtitle", "%1 - %2").arg(
          codec->toUnicode(m_artist.latin1()), codec->toUnicode(m_title.latin1())));
  }
}

#include "cdinfoencodingwidget.moc"
