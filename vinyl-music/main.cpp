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

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    if (argv[1]) {
      std::cout << "filePathArg: " << argv[1] << std::endl;
    }

    QQmlApplicationEngine engine;

    // Create custom variables accessible to QML:
    engine.rootContext()->setContextProperty("myCustomVar", QString("Hello World"));

    // Create view from main.qml:
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));


    return app.exec();
}

