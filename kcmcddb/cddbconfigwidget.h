/*
    SPDX-FileCopyrightText: 2002 Rik Hemsley (rikkus) <rik@kde.org>
    SPDX-FileCopyrightText: 2002 Benjamin Meyer <ben-devel@meyerhome.net>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CDDB_CONFIG_WIDGET_H
#define CDDB_CONFIG_WIDGET_H

#include "ui_cddbconfigwidget.h"

class CDDBConfigWidget : public QWidget, public Ui::CDDBConfigWidgetBase
{
  Q_OBJECT

  public:

    explicit CDDBConfigWidget(QWidget * parent = nullptr);

  protected Q_SLOTS:

    virtual void showMirrorList();

    virtual void protocolChanged();

    virtual void needAuthenticationChanged(bool);
};

#endif // CDDB_CONFIG_WIDGET_H
// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
