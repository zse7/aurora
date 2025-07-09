// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause
import QtQuick 2.6
import QtMultimedia 5.6
import Sailfish.Silica 1.0
import Amber.QrFilter 1.0
import Aurora.Controls 1.0

Page {
    id: recognitionPage
    objectName: "recognitionPage"

    property var appWindow

    function handleScanResult(deskNumber) {
        if (!appWindow) {
            console.log("appWindow не передан в RecognitionPage")
            return
        }
        if (appWindow.userRole === "student") {
            pageStack.push(Qt.resolvedUrl("StubPage.qml"), { deskNumber: deskNumber })
        } else if (appWindow.userRole === "teacher") {
            pageStack.push(Qt.resolvedUrl("TeacherMainPage.qml"), { deskNumber: deskNumber })
        } else {
            console.log("Роль не выбрана")
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#22333B"  // Основной цвет страницы
    }

    onVisibleChanged: {
        if (visible) {
            blackout.requestPaint(); // перерисовка затемнения
        }
    }

    // Верхний заголовок с цветом #1F252A
    Rectangle {
        id: header
        width: parent.width + 40
        height: Theme.itemSizeLarge
        color: "#1F252A"  // Цвет верхнего апбара

        Text {
            text: qsTr("Сканирование")
            anchors.centerIn: parent
            font.pixelSize: Theme.fontSizeLarge
            color: "#ECF0F1"
        }
    }

    QrFilter {
        id: qrFilter
        objectName: "qrFilter"
        active: true

        onResultChanged: {
            if (result && result.length > 0) {
                handleScanResult(result.trim())
                clearResult()
            }
        }
    }

    VideoOutput {
        id: viewer
        objectName: "viewer"
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: bottomMenu.top
        }
        fillMode: VideoOutput.PreserveAspectFit
        source: Camera {
            captureMode: Camera.CaptureVideo
            focus {
                focusMode: CameraFocus.FocusContinuous
                focusPointMode: CameraFocus.FocusPointAuto
            }
        }
        filters: [qrFilter]
    }

    Rectangle {
        id: captureRect
        objectName: "captureRect"
        anchors.centerIn: viewer
        width: Math.min(viewer.width, viewer.height) * 0.7
        height: width
        color: "transparent"

        Component.onCompleted: {
            frame.createObject(captureRect, { "x": 0, "y": 0 });
            frame.createObject(captureRect, { "x": captureRect.width, "y": 0, "rotation": 90 });
            frame.createObject(captureRect, { "x": captureRect.width, "y": captureRect.height, "rotation": 180 });
            frame.createObject(captureRect, { "x": 0, "y": captureRect.height, "rotation": -90 });
        }
    }

    // Подпись под квадратом
    Label {
        text: qsTr("Отсканируйте QR-код на парте")
        anchors.top: captureRect.bottom
        anchors.topMargin: Theme.paddingMedium
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#ECF0F1"
        font.pixelSize: Theme.fontSizeMedium - 2
    }

    Canvas {
        id: blackout
        objectName: "blackout"
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.fillStyle = Theme.rgba(Theme.overlayBackgroundColor, Theme.opacityHigh);
            ctx.beginPath();
            ctx.fillRect(viewer.x, viewer.y, viewer.width, viewer.height);
            ctx.closePath();
            ctx.fill();
            ctx.clearRect(captureRect.x, captureRect.y, captureRect.width, captureRect.height);
        }
    }

    // Нижнее меню с кликабельной иконкой QR в круге
    Rectangle {
        id: bottomMenu
        width: parent.width
        height: 140
        color: "#1F252A"  // Цвет как у верхнего апбара
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        // Фон круга (половина которого выходит за пределы панели)
        Rectangle {
            id: circleBackground
            width: 150
            height: 150
            radius: width / 2
            color: "#1F252A"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.top
            anchors.bottomMargin: -height / 1.5
        }

        // Иконка QR в круге
        Rectangle {
            id: qrCircle
            width: 80
            height: 80
            radius: width / 2
            color: "#1F252A"
            border.color: "#3A4A52"
            border.width: 2
            anchors.centerIn: circleBackground

            Image {
                id: qrIcon
                source: "../images/scan-code-qr.png"
                width: 60
                height: 60
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    qrFilter.clearResult()
                }
            }
        }
    }

    // Тестовая кнопка (перенесена выше и стилизована)
    Button {
        id: testButton
        text: "Тест: Подтверждение"
        anchors {
            right: parent.right
            bottom: bottomMenu.top  // Теперь над нижним меню
            margins: Theme.paddingLarge
        }
        width: parent.width / 2
        color: "#2A3A42"
        onClicked: handleScanResult("45")
    }

    Component {
        id: frame

        Item {
            Rectangle {
                id: verticalRect
                objectName: "verticalRect"
                anchors {
                    top: parent.top
                    left: parent.left
                }
                width: captureRect.width / 50
                height: captureRect.height / 10
                color: palette.primaryColor
            }

            Rectangle {
                objectName: "horizontalRect"
                anchors {
                    top: parent.top
                    left: verticalRect.right
                }
                width: captureRect.width / 10 - verticalRect.width
                height: captureRect.height / 50
                color: palette.primaryColor
            }
        }
    }
}
