/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>
  Copyright (C) 2003 Richard Lärkäng <nouseforaname@home.se>

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

#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qbuttongroup.h>

#include <kconfig.h>
#include <klocale.h>
#include <kglobal.h>
#include <kgenericfactory.h>
#include <kmessagebox.h>
#include <kconfigdialogmanager.h>

#include "cddbconfigwidget.h"

#include "kcmcddb.h"
#include "libkcddb/lookup.h"
#include "libkcddb/cache.h"
#include "libkcddb/submit.h"

typedef KGenericFactory<CDDBModule, QWidget> KCDDBFactory;
K_EXPORT_COMPONENT_FACTORY ( kcm_cddb, KCDDBFactory( "kcmcddb" ) )

CDDBModule::CDDBModule(QWidget *parent, const char *name, const QStringList &)
  : KCModule(parent, name)
{
  KGlobal::locale()->insertCatalogue("libkcddb");
  setButtons(Default | Apply);

  widget_ = new CDDBConfigWidget(this);

  KCDDB::Config* cfg = new KCDDB::Config();
  cfg->readConfig();

  addConfig(cfg, widget_);

  QVBoxLayout * layout = new QVBoxLayout(this, 0);

  layout->addWidget(widget_);
  layout->addStretch();

  setQuickHelp(i18n("CDDB is used to get information like artist, title and song-names in CD's"));

  load();
}

  void
CDDBModule::defaults()
{
  KCModule::defaults();

  updateWidgetsFromConfig(KCDDB::Config());
}

  void
CDDBModule::checkSettings() const
{
  KCDDB::Config config;

  config.readConfig();

  if (config.smtpHostname().isEmpty() || config.emailAddress().isEmpty()
      || !config.emailAddress().contains("@") ||
      (!config.replyTo().isEmpty() && !config.replyTo().contains("@")))
      
  {
    if (config.submitTransport() == KCDDB::Submit::SMTP)
    {
      KMessageBox::sorry(widget_, i18n("freedb submissions via SMTP have been disabled\n"
                                    "because the email details you have entered are\n"
                                    "incomplete. Please review your email settings\n"
                                    "and try again."), i18n("Freedb Submissions Disabled"));
      config.setSubmitTransport(KCDDB::Submit::None);

      config.writeConfig();
    }
  }
}

  void
CDDBModule::updateWidgetsFromConfig(const KCDDB::Config & config)
{
  widget_->fromLabel->setText(config.globalEmail());
  widget_->replyToLabel->setText(config.globalReplyTo());
  widget_->hostLabel->setText(config.globalSmtpHost());
  bool smtpUserIsEmpty = config.smtpUsername().isEmpty();
  widget_->needsAuthenticationBox->setChecked(!smtpUserIsEmpty);
  widget_->kcfg_smtpUsername->setEnabled(!smtpUserIsEmpty);

  if (config.submitTransport() != KCDDB::Submit::SMTP)
    widget_->smtpBox->setDisabled(true);
  if (!config.useGlobalEmail())
    widget_->notUseGlobalCheckbox->setChecked(true);
}

  void
CDDBModule::save()
{
  KCModule::save();

  checkSettings();
}

  void
CDDBModule::load()
{
  KCModule::load();

  KCDDB::Config config;
  config.readConfig();
  updateWidgetsFromConfig(config);
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "kcmcddb.moc"
