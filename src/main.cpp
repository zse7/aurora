// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

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

int main(int argc, char *argv[])
{
    qmlRegisterUncreatableType<QRCodeTypeClass>("ru.auroraos.QrCodeReader", 1, 0, "QRCodeType", "");
    qmlRegisterType<QRCodeHandler>("ru.auroraos.QrCodeReader", 1, 0, "QRCodeHandler");

    qmlRegisterType<CreateQrCodePageController>("ru.auroraos.QrCodeReader", 1, 0,
                                                "CreateQrCodePageController");
    qRegisterMetaType<QRCodeType>("QRCodeType");
    qRegisterMetaType<QRCodeText>("QRCodeSource");
    qRegisterMetaType<QRCodeFields>("QRCodeFields");

    QScopedPointer<QGuiApplication> application(LibApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.auroraos"));
    application->setApplicationName(QStringLiteral("QrCodeReader"));

    QScopedPointer<QQuickView> view(LibApp::createView());
    view->setSource(LibApp::pathTo(QStringLiteral("qml/QrCodeReader.qml")));
    view->show();

    return application->exec();
}
