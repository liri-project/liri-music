#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDeclarative/QDeclarativePropertyMap>
#include <QString>
#include <stdio.h>
#include <iostream>
#include <qqml.h>
#include <QQuickView>
#include <QDateTime>
#include <QQmlContext>
#include <QFileInfo>
#include <QDir>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    if (argv[1]) {
        QFileInfo finfo;
        finfo.setFile(argv[1]);

        std::cout << "filePathArg: " << argv[1] << std::endl << finfo.path().toStdString() << std::endl;
        engine.rootContext()->setContextProperty("loadedFileFolder", QString(finfo.path()));

    }

    // Get ~/Music directory
    std::string home_directory = QDir::homePath().toStdString() + std::string("/Music");
    std::string stream_directory = QDir::homePath().toStdString() + std::string("/Music/streams");

    // Create custom variables accessible to QML:
    engine.rootContext()->setContextProperty("filePathName", QString(argv[1]));
    engine.rootContext()->setContextProperty("homeDirectory", QString::fromStdString(home_directory));
    engine.rootContext()->setContextProperty("streamDirectory", QString::fromStdString(stream_directory));


    // Create view from main.qml:
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));


    return app.exec();
}

