/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2004 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cddbconfigwidget.h"
#include "kcmcddbi18n.h"

#include "config-musicbrainz.h"

#include "libkcddb/sites.h"
#include "libkcddb/lookup.h"

#include <KUrlRequester>
#include <KMessageBox>
#include <KEditListWidget>

#include <QList>
#include <QCheckBox>
#include <QInputDialog>

CDDBConfigWidget::CDDBConfigWidget(QWidget * parent)
  : QWidget(parent)
{
  setupUi(this);

#ifndef HAVE_MUSICBRAINZ5
  kcfg_MusicBrainzLookupEnabled->hide();
#endif

  // Connections from widgets are made in designer.

  KUrlRequester* urlReq = new KUrlRequester(this);
  urlReq->setMode(KFile::Directory);

  QGroupBox* groupBox = new QGroupBox(cacheLocationsParent);
  groupBox->setTitle(i18n("Cache Locations"));
  QVBoxLayout* gbLayout = new QVBoxLayout(groupBox);
  gbLayout->setContentsMargins(0, 0, 0, 0);

  KEditListWidget* editListWidget = new KEditListWidget(groupBox);
  editListWidget->setCustomEditor(urlReq->customEditor());
  editListWidget->setObjectName(QString::fromLatin1("kcfg_cacheLocations"));
  gbLayout->addWidget(editListWidget);

  QHBoxLayout *layout = new QHBoxLayout(cacheLocationsParent);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(groupBox);

  connect(needsAuthenticationBox,&QAbstractButton::toggled,this, &CDDBConfigWidget::needAuthenticationChanged);
  connect(kcfg_FreedbLookupTransport,QOverload<int>::of(&QComboBox::activated),this, &CDDBConfigWidget::protocolChanged);
  connect(kcfg_FreedbLookupEnabled,&QAbstractButton::toggled,freedbServerBox,&QWidget::setEnabled);
  connect(mirrorListButton,&QAbstractButton::clicked,this, &CDDBConfigWidget::showMirrorList);
}

void CDDBConfigWidget::showMirrorList()
{
    KCDDB::Sites s;

    const QList<KCDDB::Mirror> sites = s.siteList();
    QMap<QString, KCDDB::Mirror> keys;
    for (QList<KCDDB::Mirror>::ConstIterator it = sites.begin(); it != sites.end(); ++it)
      if ((*it).transport == KCDDB::Lookup::CDDBP)
        keys[(*it).address + QLatin1String( "(CDDBP, " ) + QString::number((*it).port) + QLatin1String( ") " ) + (*it).description] = *it;
      else
        keys[(*it).address + QLatin1String( "(HTTP, " ) + QString::number((*it).port) + QLatin1String( ") " ) + (*it).description] = *it;

    bool ok;

    if (keys.isEmpty())
    {
      KMessageBox::information(this, i18n("Could not fetch mirror list."), i18n("Could Not Fetch"));
      return;
    }

    QString result = QInputDialog::getItem(this, i18n("Select mirror"),
      i18n("Select one of these mirrors"), keys.keys(),
      0, false, &ok);

    if (ok)
    {
      KCDDB::Mirror m = keys[result];

      kcfg_FreedbLookupTransport->setCurrentIndex(m.transport == KCDDB::Lookup::CDDBP ? 0 : 1);
      kcfg_hostname->setText(m.address);
      kcfg_port->setValue(m.port);
    }
}

void CDDBConfigWidget::protocolChanged()
{
    // Change the port if the port is the default-value for the old protocol

    if (kcfg_FreedbLookupTransport->currentText() == i18n("HTTP") && kcfg_port->value() == 8880)
      kcfg_port->setValue(80);
    else if (kcfg_FreedbLookupTransport->currentText() == i18n("CDDB") && kcfg_port->value() == 80)
      kcfg_port->setValue(8880);
}

void CDDBConfigWidget::needAuthenticationChanged(bool needsAuth)
{
    kcfg_smtpUsername->setEnabled(needsAuth);
    if (!needsAuth)
      kcfg_smtpUsername->clear();
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
