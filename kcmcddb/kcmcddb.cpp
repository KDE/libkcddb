/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kcmcddb.h"
#include "kcmcddbi18n.h"
#include "cddbconfigwidget.h"

#include "libkcddb/lookup.h"
#include "libkcddb/cache.h"
#include "libkcddb/submit.h"

#include <KPluginFactory>
#include <KMessageBox>

#include <QDebug>
#include <QCheckBox>
#include <QRadioButton>
#include <QVBoxLayout>

K_PLUGIN_FACTORY(KCDDBFactory, registerPlugin<CDDBModule>();)

CDDBModule::CDDBModule(QWidget *parent, const QVariantList &args)
  : KCModule(parent, args)
{
  setButtons(Default | Apply | Help);

  widget_ = new CDDBConfigWidget(this);

  KCDDB::Config* cfg = new KCDDB::Config();
  cfg->load();

  addConfig(cfg, widget_);

  QVBoxLayout * layout = new QVBoxLayout(this);

  layout->addWidget(widget_);

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

  config.load();

  if (config.smtpHostname().isEmpty() || config.emailAddress().isEmpty()
      || !config.emailAddress().contains(QLatin1String( "@" )) ||
      (!config.replyTo().isEmpty() && !config.replyTo().contains(QLatin1String( "@" ))))

  {
    if (config.freedbSubmitTransport() == KCDDB::Submit::SMTP)
    {
      KMessageBox::error(widget_, i18n("freedb has been set to use HTTP for submissions "
                                    "because the email details you have entered are "
                                    "incomplete. Please review your email settings "
                                    "and try again."), i18n("Incorrect Email Settings"));
      config.setFreedbSubmitTransport(KCDDB::Submit::HTTP);

      config.save();
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
  config.load();
  updateWidgetsFromConfig(config);
}

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "kcmcddb.moc"
