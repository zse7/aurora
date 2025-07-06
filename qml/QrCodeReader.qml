// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.6
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow {
    id: appWindow

    readonly property string appName: qsTr("QR Code Reader")

    objectName: "appWindow"
    allowedOrientations: defaultAllowedOrientations
    cover: Qt.resolvedUrl("cover/DefaultCoverPage.qml")
    initialPage: Qt.resolvedUrl("pages/LoginPage.qml")
}
