/*
  Copyright (C) 2002 Rik Hemsley (rikkus) <rik@kde.org>
  Copyright (C) 2002 Benjamin Meyer <ben-devel@meyerhome.net>

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
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qlistbox.h>

#include <kconfig.h>
#include <klocale.h>
#include <kglobal.h>

#include "cddbconfigwidget.h"

#include "kcmcddb.h"
#include <libkcddb/lookup.h>
#include <libkcddb/cache.h>

extern "C"
{
    KCModule *
  create_cddb_config(QWidget * parent, const char * name)
  {
    KGlobal::locale()->insertCatalogue("kcmcddb");
    return new CDDBModule(parent, name);
  }
}

CDDBModule::CDDBModule(QWidget * parent, const char * name)
  : KCModule(parent, name)
{
  setButtons(Default | Apply);

  widget_ = new CDDBConfigWidget(this);
  
  QVBoxLayout * layout = new QVBoxLayout(this, 0);

  layout->addWidget(widget_);

  load();

  connect(widget_, SIGNAL(configChanged()), SLOT(slotConfigChanged()));

  emit(changed(false));
}

  void
CDDBModule::defaults()
{
  updateWidgetsFromConfig(KCDDB::Config());

  emit(changed(readConfigFromWidgets() != originalConfig_));
}

  KCDDB::Config
CDDBModule::readConfigFromWidgets() const
{
  KCDDB::Config config;

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
  config.setEmailAddress        (widget_->submissionsSendTo->text());
  config.setSubmissionsEnabled  (widget_->submissionsEnable->isChecked());
  config.setLookupTransport     (cddbLookup ?
                                KCDDB::CDDB::CDDBP : KCDDB::CDDB::HTTP);
  config.setCachePolicy         (policy);

  QStringList l;
  for (uint i=0; i < widget_->cacheDirectories->count(); i++)
    l.append(widget_->cacheDirectories->text(i));

  config.setCacheLocations(l);

  return config;
}

  void
CDDBModule::updateWidgetsFromConfig(const KCDDB::Config & config)
{
  bool cddbLookup = (config.lookupTransport() == KCDDB::CDDB::CDDBP);

  widget_->cddbType           ->setCurrentItem  (cddbLookup ? 0 : 1);
  widget_->cddbServer         ->setText         (config.hostname());
  widget_->cddbPort           ->setValue        (config.port());
  widget_->submissionsEnable  ->setChecked      (config.submissionsEnabled());
  widget_->submissionsSendTo  ->setText         (config.emailAddress());
  if (config.cachePolicy() == KCDDB::Cache::Only)
    widget_->cacheOnly->setChecked(true);
  else if (config.cachePolicy() == KCDDB::Cache::Use)
    widget_->cacheAndRemote->setChecked(true);
  else
    widget_->remoteOnly->setChecked(true);
  widget_->cacheDirectories   ->clear();
  widget_->cacheDirectories   ->insertStringList(config.cacheLocations());
}

  void
CDDBModule::save()
{
  KCDDB::Config newConfig(readConfigFromWidgets());

  if (newConfig != originalConfig_)
    newConfig.save();
}

  void
CDDBModule::load()
{
  originalConfig_.load();
  updateWidgetsFromConfig(originalConfig_);
}

  void
CDDBModule::slotConfigChanged()
{
  emit(changed(readConfigFromWidgets() != originalConfig_));
}

  QString
CDDBModule::quickHelp() const
{
  return i18n
    (
      "<h1>TODO: write this help</h1>"
    );
}


// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1

#include "kcmcddb.moc"
