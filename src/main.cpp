#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

#ifdef USING_SAILFISHAPP
#  include <sailfishapp.h>
#else
#  include <auroraapp.h>
#endif

#ifdef USING_SAILFISHAPP
namespace LibApp = SailfishApp;
#else
namespace LibApp = Aurora::Application;
#endif

#include "handler/qrcodehandler.h"
#include "createqrcodepagecontroller.h"

// ➕ Подключаем новый заголовок
#include "types/variantdistributor.h"

int main(int argc, char *argv[])
{
    // Регистрация существующих классов
    qmlRegisterUncreatableType<QRCodeTypeClass>("ru.auroraos.QrCodeReader", 1, 0, "QRCodeType", "");
    qmlRegisterType<QRCodeHandler>("ru.auroraos.QrCodeReader", 1, 0, "QRCodeHandler");
    qmlRegisterType<CreateQrCodePageController>("ru.auroraos.QrCodeReader", 1, 0, "CreateQrCodePageController");

    // ➕ Регистрация нового класса для QML
    qmlRegisterType<VariantDistributor>("ru.auroraos.QrCodeReader", 1, 0, "VariantDistributor");

    // Регистрация типов данных
    qRegisterMetaType<QRCodeType>("QRCodeType");
    qRegisterMetaType<QRCodeText>("QRCodeSource");
    qRegisterMetaType<QRCodeFields>("QRCodeFields");

    // Создание приложения
    QScopedPointer<QGuiApplication> application(LibApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.auroraos"));
    application->setApplicationName(QStringLiteral("QrCodeReader"));

    // Создание окна
    QScopedPointer<QQuickView> view(LibApp::createView());
    view->setSource(LibApp::pathTo(QStringLiteral("qml/QrCodeReader.qml")));
    view->show();

    return application->exec();
}
