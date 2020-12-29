/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KCMCDDB_H
#define KCMCDDB_H

#include <KConfigWidgets/KCModule>
#include "libkcddb/kcddbconfig.h"

class CDDBConfigWidget;


class CDDBModule : public KCModule
{
  Q_OBJECT

  public:

	CDDBModule(QWidget * parent, const QVariantList & args);

  public Q_SLOTS:

    void defaults() override;
    void save() override;
    void load() override;

  protected:

    void            checkSettings() const;
    void            updateWidgetsFromConfig(const KCDDB::Config &);

  private:

    CDDBConfigWidget  * widget_;
};

#endif // KCMCDDB_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
