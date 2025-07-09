import QtQuick 2.6
import Sailfish.Silica 1.0

Page {
    id: conductTestingPage
    objectName: "conductTestingPage"

    // Параметры страницы
    property int studentCount: 0

    Rectangle {
        anchors.fill: parent
        color: "#22333B"
    }

    // Верхний заголовок (апбар)
    Rectangle {
        width: parent.width + 40
        height: Theme.itemSizeLarge
        color: "#1F252A"

        Text {
            text: qsTr("Провести тестирование")
            anchors.centerIn: parent
            font.pixelSize: Theme.fontSizeLarge
            color: "#ECF0F1"
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: mainColumn.implicitHeight

        Column {
            id: mainColumn
            width: parent.width * 0.8
            spacing: Theme.paddingLarge * 2
            anchors.horizontalCenter: parent.horizontalCenter

            // ВЕРХНЯЯ ПРОКЛАДКА для центрирования
            Rectangle {
                width: 1
                height: (parent.height - mainColumn.implicitHeight) / 2
                color: "transparent"
            }

            // СОДЕРЖИМОЕ
            Label {
                text: qsTr("Выберите тему теста:")
                color: "#ECF0F1"
                font.pixelSize: Theme.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            ComboBox {
                id: topicCombo
                width: parent.width * 0.85
                label: qsTr("Темы тестов")
                menu: ContextMenu {
                    MenuItem { text: "Математика" }
                    MenuItem { text: "Информатика" }
                    MenuItem { text: "Физика" }
                }
                onCurrentIndexChanged: {
                    testTopicField.text = topicCombo.currentItem ? topicCombo.currentItem.text : ""
                }
            }

            TextField {
                id: testTopicField
                width: parent.width * 0.85
                placeholderText: qsTr("Или введите тему вручную")
                color: "#ECF0F1"
                placeholderColor: "#7F8C8D"
                horizontalAlignment: Text.AlignLeft
            }

            Label {
                text: qsTr("Количество студентов: ") + conductTestingPage.studentCount
                color: "#ECF0F1"
                font.pixelSize: Theme.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Rectangle {
                width: parent.width * 0.7
                height: Theme.itemSizeExtraLarge - 50
                radius: height / 2
                color: "#1F252A"
                border.color: "#3A4A52"
                border.width: 2

                Label {
                    text: qsTr("Начать тестирование")
                    anchors.centerIn: parent
                    color: "#ECF0F1"
                    font.pixelSize: Theme.fontSizeLarge
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var topic = testTopicField.text.length > 0 ? testTopicField.text : (topicCombo.currentItem ? topicCombo.currentItem.text : "")
                        if (topic.length === 0) {
                            showBanner(qsTr("Выберите или введите тему"))
                            return
                        }
                        pageStack.push(Qt.resolvedUrl("StudentsVariantsPage.qml"), {
                            topic: topic,
                            studentCount: conductTestingPage.studentCount
                        })
                    }
                }
                Rectangle {
                    width: 1
                    height: (parent.height - mainColumn.implicitHeight) / 2
                    color: "transparent"
                }
            }
        }
    }
}
