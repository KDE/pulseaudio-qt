/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef CLIENT_H
#define CLIENT_H

#include "kf5pulseaudioqt_export.h"
#include "indexedpulseobject.h"

struct pa_client_info;

namespace PulseAudioQt
{
class KF5PULSEAUDIOQT_EXPORT Client : public IndexedPulseObject
{
    Q_OBJECT
public:
    ~Client();

private:
    explicit Client(QObject *parent);

    class ClientPrivate *const d;
    friend class MapBase<Client, pa_client_info>;
};

} // PulseAudioQt

#endif // CLIENT_H
