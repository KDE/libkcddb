/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2004 Richard Lärkäng <nouseforaname@home.se>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "cddbconfigwidget.h"

#include "libkcddb/sites.h"
#include "libkcddb/lookup.h"

#include <qlistbox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <kfiledialog.h>
#include <kapplication.h>
#include <klocale.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <keditlistbox.h>
#include <qwidgetstack.h>
#include <kurlrequester.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>

CDDBConfigWidget::CDDBConfigWidget(QWidget * parent, const char * name)
  : CDDBConfigWidgetBase(parent, name)
{
  // Connections from widgets are made in designer.

  KURLRequester* urlReq = new KURLRequester(this);
  urlReq->setMode(KFile::Directory);

  KEditListBox* editListBox = new KEditListBox(i18n("Cache Locations"), urlReq->customEditor(), cacheLocationsParent, "kcfg_cacheLocations");
  cacheLocationsParent->raiseWidget(editListBox);

  kcfg_submitTransport->remove(needsAuthenticationBox);
}

void CDDBConfigWidget::showMirrorList()
{
    KCDDB::Sites s;

    QValueList<KCDDB::Mirror> sites = s.siteList();
    QMap<QString, KCDDB::Mirror> keys;
    for (QValueList<KCDDB::Mirror>::Iterator it = sites.begin(); it != sites.end(); ++it)
      if ((*it).transport == KCDDB::Lookup::CDDBP)
        keys[(*it).address + "(CDDBP, " + QString::number((*it).port) + ") " + (*it).description] = *it;
      else
        keys[(*it).address + "(HTTP, " + QString::number((*it).port) + ") " + (*it).description] = *it;

    bool ok;

    if (keys.isEmpty())
    {
      KMessageBox::information(this, i18n("Could not fetch mirror list."), i18n("Could Not Fetch"));
      return;
    }

    QStringList result = KInputDialog::getItemList(i18n("Select mirror"),
      i18n("Select one of these mirrors"), keys.keys(),
      QStringList(), false, &ok, this);

    if (ok && result.count() == 1)
    {
      KCDDB::Mirror m = keys[*(result.begin())];

      kcfg_lookupTransport->setCurrentItem(m.transport == KCDDB::Lookup::CDDBP ? 0 : 1);
      kcfg_hostname->setText(m.address);
      kcfg_port->setValue(m.port);
    }
}

void CDDBConfigWidget::protocolChanged()
{
    // Change the port if the port is the default-value for the old protocol

    if (kcfg_lookupTransport->currentText() == i18n("HTTP") && kcfg_port->value() == 8880)
      kcfg_port->setValue(80);
    else if (kcfg_lookupTransport->currentText() == i18n("CDDB") && kcfg_port->value() == 80)
      kcfg_port->setValue(8880);
}

void CDDBConfigWidget::needAuthenticationChanged(bool needsAuth)
{
    kcfg_smtpUsername->setEnabled(needsAuth);
    if (!needsAuth)
      kcfg_smtpUsername->clear();
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "cddbconfigwidget.moc"
