#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "tablemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setAttribute(Qt::AA_EnableHighDpiScaling);

    QQmlApplicationEngine engine;
    
    TableModel model;
    engine.rootContext()->setContextProperty("tableModel", &model);
    
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
