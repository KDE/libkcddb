/*
 * SPDX-FileCopyrightText: 2026 Nate Graham <nate@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "kcmcddbmoduledata.h"

#include <Solid/Device>
#include <Solid/DeviceNotifier>
#include <Solid/OpticalDrive>

CDDBModuleModuleData::CDDBModuleModuleData(QObject *parent)
    : KCModuleData(parent)
{
    connect(Solid::DeviceNotifier::instance(), &Solid::DeviceNotifier::deviceAdded, this, &CDDBModuleModuleData::updateRelevance);
    connect(Solid::DeviceNotifier::instance(), &Solid::DeviceNotifier::deviceRemoved, this, &CDDBModuleModuleData::updateRelevance);

    updateRelevance();
}

void CDDBModuleModuleData::updateRelevance()
{
    const auto opticalDrives = Solid::Device::listFromType(Solid::DeviceInterface::OpticalDrive);
    setRelevant(opticalDrives.count() > 0);
}

#include "moc_kcmcddbmoduledata.cpp"
