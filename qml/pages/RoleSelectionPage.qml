import QtQuick 2.6
import Sailfish.Silica 1.0

Page {
    id: roleSelectionPage

    Rectangle {
        anchors.fill: parent
        color: "#22333B"
    }

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingLarge * 2  // Увеличил расстояние между кнопками
        width: parent.width * 0.8

        // Кнопка "Я ученик"
        Rectangle {
            width: parent.width
            height: Theme.itemSizeExtraLarge - 50  // Увеличил высоту
            radius: height / 2  // Сильное скругление (половина высоты)
            color: "#1F252A"  // Статичный цвет
            border.color: "#3A4A52"
            border.width: 2

            Label {
                text: qsTr("Я студент")
                anchors.centerIn: parent
                color: "#ECF0F1"
                font {
                    pixelSize: Theme.fontSizeLarge
                    bold: true  // Жирный шрифт
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: pageStack.push(Qt.resolvedUrl("RecognitionPage.qml"))
            }
        }

        // Кнопка "Я преподаватель"
        Rectangle {
            width: parent.width
            height: Theme.itemSizeExtraLarge - 50
            radius: height / 2  // Аналогичное сильное скругление
            color: "#1F252A"
            border.color: "#3A4A52"
            border.width: 2

            Label {
                text: qsTr("Я преподаватель")
                anchors.centerIn: parent
                color: "#ECF0F1"
                font {
                    pixelSize: Theme.fontSizeLarge
                    bold: true
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: pageStack.push(Qt.resolvedUrl("TeacherPage.qml"))
            }
        }
    }
}
