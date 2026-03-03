// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
// SPDX-FileCopyrightText: 2026 Harald Sitter <sitter@kde.org>

#include <QCoreApplication>

#include "context.h"
#include "models.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    app.setQuitLockEnabled(true);

    auto context = PulseAudioQt::Context::instance();

    {
        auto lock = std::make_shared<QEventLoopLocker>();
        QObject::connect(context, &PulseAudioQt::Context::stateChanged, &app, [=]() mutable {
            if (context->state() == PulseAudioQt::Context::State::Ready) {
                lock.reset();
            }
        });
    }

    {
        auto lock = std::make_shared<QEventLoopLocker>();
        QObject::connect(context, &PulseAudioQt::Context::cardAdded, &app, [=]() mutable {
            lock.reset();
        });
    }

    PulseAudioQt::CardModel cardModel;
    {
        auto lock = std::make_shared<QEventLoopLocker>();
        QObject::connect(&cardModel, &PulseAudioQt::CardModel::countChanged, &app, [lock, &cardModel]() mutable {
            if (cardModel.rowCount() > 0) {
                lock.reset();
            }
        });
    }

    return app.exec();
}
