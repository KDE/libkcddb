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

#ifndef KCMCDDB_H
#define KCMCDDB_H

#include <kcmodule.h>

#include <libkcddb/config.h>

class CDDBConfigWidgetBase;

class CDDBModule : public KCModule
{
  Q_OBJECT

  public:

    CDDBModule(QWidget * parent = 0, const char * name = 0);

    QString quickHelp() const;

  public slots:

    void defaults();
    void save();
    void load();

  protected:

    KCDDB::Config   readConfigFromWidgets() const;
    void            updateWidgetsFromConfig(const KCDDB::Config &);

  private slots:

    void slotConfigChanged();

  private:

    CDDBConfigWidgetBase  * widget_;
    KCDDB::Config           originalConfig_;
};

#endif // KCMCDDB_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
