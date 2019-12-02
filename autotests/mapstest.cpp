/*
    Copyright 2018 David Rosca <nowrep@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mapstest.h"

#include "maps.h"
#include "module.h"

#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

using namespace PulseAudioQt;

void MapsTest::initTestCase()
{
}

void MapsTest::cleanupTestCase()
{
}

static pa_module_info *module_info_new()
{
    pa_module_info *info = (pa_module_info *)calloc(1, sizeof(pa_module_info));
    info->name = "name";
    info->argument = "arg";
    info->n_used = PA_INVALID_INDEX;
    info->proplist = pa_proplist_new();
    return info;
}

static void module_info_free(pa_module_info *info)
{
    pa_proplist_free(info->proplist);
    free(info);
}

void MapsTest::basicTest()
{
    QObject p;
    ModuleMap map;

    pa_module_info *info = module_info_new();
    info->index = 10;

    QCOMPARE(map.count(), 0);

    map.updateEntry(info, &p);
    QCOMPARE(map.count(), 1);
    map.updateEntry(info, &p);
    QCOMPARE(map.count(), 1);

    QObject *index10 = map.objectAt(0);
    QVERIFY(index10);

    QCOMPARE(map.indexOfObject(index10), 0);

    // Add 0 (10, 0)
    info->index = 0;
    map.updateEntry(info, &p);
    QCOMPARE(map.count(), 2);
    QCOMPARE(map.objectAt(0), index10);

    QObject *index0 = map.objectAt(1);
    QVERIFY(index0);

    QCOMPARE(map.indexOfObject(index0), 1);
    QCOMPARE(map.indexOfObject(index10), 0);

    // Add 4, 15 (10, 0, 4, 15)
    info->index = 4;
    map.updateEntry(info, &p);
    info->index = 15;
    map.updateEntry(info, &p);

    QCOMPARE(map.count(), 4);
    QCOMPARE(map.objectAt(0), index10);
    QCOMPARE(map.objectAt(1), index0);

    QObject *index4 = map.objectAt(2);
    QVERIFY(index4);
    QObject *index15 = map.objectAt(3);
    QVERIFY(index15);

    QCOMPARE(map.indexOfObject(index10), 0);
    QCOMPARE(map.indexOfObject(index0), 1);
    QCOMPARE(map.indexOfObject(index4), 2);
    QCOMPARE(map.indexOfObject(index15), 3);

    // Remove 4 (10, 0, 15)
    map.removeEntry(4);
    QCOMPARE(map.count(), 3);
    QCOMPARE(map.objectAt(0), index10);
    QCOMPARE(map.objectAt(1), index0);
    QCOMPARE(map.objectAt(2), index15);

    // Remove 0 (10, 15)
    map.removeEntry(0);
    QCOMPARE(map.count(), 2);
    QCOMPARE(map.objectAt(0), index10);
    QCOMPARE(map.objectAt(1), index15);

    // Remove 15 (10)
    map.removeEntry(15);
    QCOMPARE(map.count(), 1);
    QCOMPARE(map.objectAt(0), index10);

    // Remove last ()
    map.removeEntry(10);
    QCOMPARE(map.count(), 0);

    module_info_free(info);
}

void MapsTest::pendingRemovalsTest()
{
    QObject p;
    ModuleMap map;

    pa_module_info *info = module_info_new();
    info->index = 10;

    QCOMPARE(map.count(), 0);

    // 10 is not in map, should set pending removal
    map.removeEntry(10);
    // 10 is in pending removals, this should eat it and not add it into map
    map.updateEntry(info, &p);
    QCOMPARE(map.count(), 0);

    // No more pending removals, this should add it
    map.updateEntry(info, &p);
    QCOMPARE(map.count(), 1);

    module_info_free(info);
}

QTEST_MAIN(MapsTest)
