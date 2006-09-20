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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <qradiobutton.h>
#include <q3listbox.h>
#include <QLabel>
#include <q3buttongroup.h>
//Added by qt3to4:
#include <QVBoxLayout>

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
K_EXPORT_COMPONENT_FACTORY ( cddb, KCDDBFactory( "kcmcddb" ) )

CDDBModule::CDDBModule(QWidget *parent, const QStringList &)
  : KCModule(KCDDBFactory::instance(), parent)
{
  KGlobal::locale()->insertCatalog("libkcddb");
  setButtons(Default | Apply);

  widget_ = new CDDBConfigWidget(this);

  KCDDB::Config* cfg = new KCDDB::Config();
  cfg->readConfig();

  addConfig(cfg, widget_);

  QVBoxLayout * layout = new QVBoxLayout(this);
  layout->setSpacing(0);

  layout->addWidget(widget_);
  layout->addStretch();

  setQuickHelp(i18n("CDDB is used to get information like artist, title and song-names in CD's"));

  load();
}

  void
CDDBModule::defaults()
{
  KCModule::defaults();

  KCDDB::Config config;
  updateWidgetsFromConfig( config );
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
      KMessageBox::sorry(widget_, i18n("freedb has been set to use HTTP for submissions "
                                    "because the email details you have entered are "
                                    "incomplete. Please review your email settings "
                                    "and try again."), i18n("Incorrect Email Settings"));
      config.setSubmitTransport(KCDDB::Submit::HTTP);

      config.writeConfig();
    }
  }
}

  void
CDDBModule::updateWidgetsFromConfig(const KCDDB::Config & config)
{
  bool smtpUserIsEmpty = config.smtpUsername().isEmpty();
  widget_->needsAuthenticationBox->setChecked(!smtpUserIsEmpty);
  widget_->kcfg_smtpUsername->setEnabled(!smtpUserIsEmpty);
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
