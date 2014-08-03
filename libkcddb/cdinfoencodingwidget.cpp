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

#include "cdinfoencodingwidget.h"

#include <qtextcodec.h>
#include <klocale.h>
#include <kglobal.h>
#include <kcharsets.h>

namespace KCDDB
{
  CDInfoEncodingWidget::CDInfoEncodingWidget(QWidget* parent, const QString& artist,
          const QString& title, const QStringList& songTitles)
    : QWidget(parent), CDInfoEncodingWidgetBase(), m_artist(artist), m_title(title),
        m_songTitles(songTitles)
  {
    setupUi(this);

    encodingCombo->addItems(KGlobal::charsets()->descriptiveEncodingNames());

    slotEncodingChanged(encodingCombo->currentText());

    connect(encodingCombo,SIGNAL(activated(QString)),
        this,SLOT(slotEncodingChanged(QString)));
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

    for (QStringList::const_iterator it = m_songTitles.constBegin();
        it != m_songTitles.constEnd(); ++it)
      newTitles << codec->toUnicode((*it).toLatin1().constData());

    songsBox->clear();
    songsBox->addItems(newTitles);

    titleLabel->setText(i18nc("artist - cdtitle", "%1 - %2",
          codec->toUnicode(m_artist.toLatin1()), codec->toUnicode(m_title.toLatin1().constData())));
  }
}

