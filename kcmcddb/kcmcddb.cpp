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
  setButtons(Default | Apply);

  widget_ = new CDDBConfigWidget(this);

  QVBoxLayout * layout = new QVBoxLayout(this, 0);

  layout->addWidget(widget_);
  layout->addStretch();

  load();

  connect(widget_, SIGNAL(configChanged()), SLOT(slotConfigChanged()));
}

  void
CDDBModule::defaults()
{
  updateWidgetsFromConfig(KCDDB::Config());

  setChanged(true);
}

  void
CDDBModule::readConfigFromWidgets(KCDDB::Config &config) const
{
  bool cddbLookup = (0 == widget_->cddbType->currentItem());
  KCDDB::Cache::Policy policy;
  if (widget_->cacheOnly->isChecked())
    policy = KCDDB::Cache::Only;
  else if (widget_->cacheAndRemote->isChecked())
    policy = KCDDB::Cache::Use;
  else
    policy = KCDDB::Cache::Ignore;

  config.setHostname            (widget_->cddbServer->text());
  config.setPort                (widget_->cddbPort->value());
  config.setLookupTransport     (cddbLookup ?
                                KCDDB::Lookup::CDDBP : KCDDB::Lookup::HTTP);
  config.setCachePolicy         (policy);

  QStringList l;
  for (uint i=0; i < widget_->cacheDirectories->count(); i++)
    l.append(widget_->cacheDirectories->text(i));

  config.setCacheLocations(l);

  config.setOwnEmail(widget_->fromLineEdit->text());
  config.setOwnReplyTo(widget_->replyToLineEdit->text());
  config.setOwnSmtpHost(widget_->hostLineEdit->text());
  config.setSmtpPort(widget_->portSpinBox->value());
  config.setSmtpUsername(widget_->usernameLineEdit->text());
  config.setUseGlobalEmail(widget_->useGlobalCheckbox->isChecked());
  if (widget_->enableSmtpCheckBox->isChecked())
    config.setSubmitTransport(KCDDB::Submit::SMTP);
  else
    config.setSubmitTransport(KCDDB::Submit::None);

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
    }
  }
}

  void
CDDBModule::updateWidgetsFromConfig(const KCDDB::Config & config)
{
  bool cddbLookup = (config.lookupTransport() == KCDDB::Lookup::CDDBP);

  widget_->cddbType           ->setCurrentItem  (cddbLookup ? 0 : 1);
  widget_->cddbServer         ->setText         (config.hostname());
  widget_->cddbPort           ->setValue        (config.port());
  if (config.cachePolicy() == KCDDB::Cache::Only)
    widget_->cacheOnly->setChecked(true);
  else if (config.cachePolicy() == KCDDB::Cache::Use)
    widget_->cacheAndRemote->setChecked(true);
  else
    widget_->remoteOnly->setChecked(true);
  widget_->cacheDirectories   ->clear();
  widget_->cacheDirectories   ->insertStringList(config.cacheLocations());
  widget_->fromLabel->setText(config.globalEmail());
  widget_->replyToLabel->setText(config.globalReplyTo());
  widget_->hostLabel->setText(config.globalSmtpHost());
  widget_->fromLineEdit->setText(config.ownEmail());
  widget_->replyToLineEdit->setText(config.ownReplyTo());
  widget_->hostLineEdit->setText(config.ownSmtpHost());
  widget_->portSpinBox->setValue(config.smtpPort());
  widget_->usernameLineEdit->setText(config.smtpUsername());
  if (config.submitTransport() == KCDDB::Submit::SMTP)
    widget_->enableSmtpCheckBox->setChecked(true);
  else
    widget_->smtpBox->setDisabled(true);
  if (config.useGlobalEmail())
    widget_->useGlobalCheckbox->setChecked(true);
  else
    widget_->notUseGlobalCheckbox->setChecked(true);
}

  void
CDDBModule::save()
{
  KCDDB::Config newConfig;

  newConfig.readConfig();

  readConfigFromWidgets(newConfig);

  newConfig.writeConfig();
  setChanged(false);
}

  void
CDDBModule::load()
{
  originalConfig_.readConfig();
  updateWidgetsFromConfig(originalConfig_);
}

  void
CDDBModule::slotConfigChanged()
{
  setChanged(true);
}

  QString
CDDBModule::quickHelp() const
{
  return i18n
    (
      "CDDB is used to get information like artist, title and song-names in CD's"
    );
}


// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "kcmcddb.moc"
