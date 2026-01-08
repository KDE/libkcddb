/*
 * SPDX-FileCopyrightText: 2026 Nate Graham <nate@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

#include <KCModuleData>

class CDDBModuleModuleData : public KCModuleData
{
    Q_OBJECT

public:
    CDDBModuleModuleData(QObject *parent = nullptr);

private:
    void updateRelevance();
};
