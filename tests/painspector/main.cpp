/*
    SPDX-FileCopyrightText: 2018 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "models.h"
#include "port.h"

class Enums : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE QString portTypeToString(int type)
    {
        QMetaObject metaObject = PulseAudioQt::Port::staticMetaObject;
        QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("Type"));

        return QString(metaEnum.valueToKey(type));
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const char *uri = "org.kde.pulseaudioqt.tests";

    qmlRegisterType<PulseAudioQt::CardModel>(uri, 0, 1, "CardModel");
    qmlRegisterType<PulseAudioQt::SinkModel>(uri, 0, 1, "SinkModel");
    qmlRegisterType<PulseAudioQt::SinkInputModel>(uri, 0, 1, "SinkInputModel");
    qmlRegisterType<PulseAudioQt::SourceModel>(uri, 0, 1, "SourceModel");
    qmlRegisterType<PulseAudioQt::SourceOutputModel>(uri, 0, 1, "SourceOutputModel");
    qmlRegisterType<PulseAudioQt::StreamRestoreModel>(uri, 0, 1, "StreamRestoreModel");
    qmlRegisterType<PulseAudioQt::ModuleModel>(uri, 0, 1, "ModuleModel");

    Enums e;
    qmlRegisterSingletonInstance("org.kde.pulseaudioqt.painspector", 1, 0, "Enums", &e);

    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}

#include "main.moc"
