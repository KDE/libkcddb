/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KCMCDDB_H
#define KCMCDDB_H

#include <KCModule>
#include "libkcddb/config.h"

class CDDBConfigWidget;
class KPluginMetaData;


class CDDBModule : public KCModule
{
  Q_OBJECT

  public:

    CDDBModule(QObject * parent, const KPluginMetaData &metaData);

  public Q_SLOTS:

    void defaults() override;
    void save() override;
    void load() override;

  protected:
    void checkSettings() const;

  private:

    CDDBConfigWidget  * widget_;
};

#endif // KCMCDDB_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
