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

#include <QCheckBox>
#include <QVBoxLayout>

K_PLUGIN_CLASS_WITH_JSON(CDDBModule, "kcm_cddb.json")

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
CDDBModule::CDDBModule(QWidget *parent, const QVariantList &args)
  : KCModule(parent, args)
#else
CDDBModule::CDDBModule(QObject *parent, const KPluginMetaData &metaData, const QVariantList &args)
  : KCModule(parent, metaData, args)
#endif
{
  setButtons(Default | Apply | Help);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  widget_ = new CDDBConfigWidget(this);
#else
  widget_ = new CDDBConfigWidget(widget());
#endif

  KCDDB::Config* cfg = new KCDDB::Config();
  cfg->load();

  addConfig(cfg, widget_);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QVBoxLayout * layout = new QVBoxLayout(this);
#else
  QVBoxLayout * layout = new QVBoxLayout(widget());
#endif

  layout->addWidget(widget_);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  setQuickHelp(i18n("CDDB is used to get information like artist, title and song-names in CD's"));
#endif

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
