#include <QtQml>
#include <QtQml/QQmlContext>
#include "backend.h"
#include "scanimage.h"

void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Doc_Scanner"));

    qmlRegisterType<ScanImage>(uri, 1, 0, "ScanImage");
}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
