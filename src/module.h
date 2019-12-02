/*
    Copyright 2017 David Rosca <nowrep@gmail.com>

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

#ifndef MODULE_H
#define MODULE_H

#include "kf5pulseaudioqt_export.h"
#include "pulseobject.h"

struct pa_module_info;

namespace PulseAudioQt
{
class KF5PULSEAUDIOQT_EXPORT Module : public PulseObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString argument READ argument NOTIFY argumentChanged)

public:
    ~Module();

    QString name() const;
    QString argument() const;

Q_SIGNALS:
    void nameChanged();
    void argumentChanged();

private:
    explicit Module(QObject *parent);

    class ModulePrivate *const d;
    friend class MapBase<Module, pa_module_info>;
};

} // PulseAudioQt

#endif // MODULE_H
