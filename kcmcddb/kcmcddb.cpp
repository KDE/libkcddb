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

  load();

  connect(widget_, SIGNAL(configChanged()), SLOT(slotConfigChanged()));
}

  void
CDDBModule::defaults()
{
  updateWidgetsFromConfig(KCDDB::Config());

  KCModule::defaults();

  emit changed(true);
}

  void
CDDBModule::readConfigFromWidgets(KCDDB::Config &config) const
{
  if (widget_->needsAuthenticationBox->isChecked())
    config.setSmtpUsername(widget_->usernameLineEdit->text());
  else
    config.setSmtpUsername(QString::null);
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
  widget_->fromLabel->setText(config.globalEmail());
  widget_->replyToLabel->setText(config.globalReplyTo());
  widget_->hostLabel->setText(config.globalSmtpHost());
  if (!config.smtpUsername().isEmpty())
  {
    widget_->needsAuthenticationBox->setChecked(true);
    widget_->usernameLineEdit->setEnabled(true);
    widget_->usernameLineEdit->setText(config.smtpUsername());
  }
  else
  {
    widget_->usernameLineEdit->setEnabled(false);
    widget_->usernameLineEdit->setText(QString::null);
  }

  if (config.submitTransport() == KCDDB::Submit::SMTP)
    widget_->enableSmtpCheckBox->setChecked(true);
  else
    widget_->smtpBox->setDisabled(true);
  if (!config.useGlobalEmail())
    widget_->notUseGlobalCheckbox->setChecked(true);
}

  void
CDDBModule::save()
{
  KCDDB::Config newConfig;

  newConfig.readConfig();

  readConfigFromWidgets(newConfig);

  newConfig.writeConfig();

  KCModule::save();

  emit changed(false);
}

  void
CDDBModule::load()
{
  originalConfig_.readConfig();
  updateWidgetsFromConfig(originalConfig_);

  KCModule::load();
}

  void
CDDBModule::slotConfigChanged()
{
  emit changed(true);
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
