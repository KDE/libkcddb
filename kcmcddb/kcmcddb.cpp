/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>
    SPDX-FileCopyrightText: 2003 Richard Lärkäng <nouseforaname@home.se>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kcmcddb.h"
#include "kcmcddbi18n.h"
#include "cddbconfigwidget.h"
#include "kcmcddbmoduledata.h"

#include "libkcddb/lookup.h"
#include "libkcddb/cache.h"
#include "libkcddb/submit.h"

#include <KPluginFactory>
#include <KMessageBox>

#include <QCheckBox>
#include <QVBoxLayout>

K_PLUGIN_FACTORY_WITH_JSON(CDDBModuleFactory, "kcm_cddb.json", registerPlugin<CDDBModule>(); registerPlugin<CDDBModuleModuleData>();)

CDDBModule::CDDBModule(QObject *parent, const KPluginMetaData &metaData)
  : KCModule(parent, metaData)
{
  setButtons(Default | Apply | Help);

  widget_ = new CDDBConfigWidget(widget());

  KCDDB::Config* cfg = new KCDDB::Config();
  cfg->load();

  addConfig(cfg, widget_);

  QVBoxLayout * layout = new QVBoxLayout(widget());
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(widget_);

  load();
}

  void
CDDBModule::defaults()
{
  KCModule::defaults();

  KCDDB::Config config;
}

  void
CDDBModule::checkSettings() const
{
  KCDDB::Config config;

  config.load();
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
}

#include "kcmcddb.moc"
#include "moc_kcmcddb.cpp"

// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
