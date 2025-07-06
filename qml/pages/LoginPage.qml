import QtQuick 2.6
import Sailfish.Silica 1.0

Page {
    id: loginPage

    Rectangle {
        anchors.fill: parent
        color: "#22333B"
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: mainColumn.implicitHeight

        Column {
            id: mainColumn
            width: parent.width
            spacing: Theme.paddingLarge   // ➕ Расстояние между крупными блоками
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.3   // ➕ Круг ниже

            // Круглая иконка с картинкой
            Rectangle {
                width: 220
                height: 220
                radius: width / 2
                color: "#1F252A"
                opacity: 0.15
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "#BDC3C7"
                border.width: 2

                Image {
                    width: 150    // ➕ Фиксированный размер изображения
                    height: 150
                    anchors.centerIn: parent
                    source: "../images/education.png"    // Путь к вашей иконке (поместите в папку images)
                    fillMode: Image.PreserveAspectFit
                }
            }

            // ➕ Spacer для увеличения расстояния между кругом и полями
            Item {
                width: 1
                height: Theme.paddingLarge * 1.5   // ➕ Больше отступа
            }

            // Колонка с полями ввода
            Column {
                width: parent.width
                spacing: Theme.paddingSmall * 0.8    // ➕ Ближе поля «Имя» и «Пароль»
                anchors.horizontalCenter: parent.horizontalCenter

                TextField {
                    id: usernameField
                    width: parent.width * 0.8
                    placeholderText: qsTr("Имя пользователя")
                    color: "#ECF0F1"
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignLeft
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                TextField {
                    id: passwordField
                    width: parent.width * 0.8
                    placeholderText: qsTr("Пароль")
                    echoMode: TextInput.Password
                    color: "#ECF0F1"
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignLeft
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            // ➕ Spacer между полями и кнопками
            Item {
                width: 1
                height: Theme.paddingLarge * 1    // ➕ Поля и кнопки дальше друг от друга
            }

            // Колонка с кнопками
            Column {
                width: parent.width
                spacing: 40   // ➕ Кнопки ближе между собой
                anchors.horizontalCenter: parent.horizontalCenter

                // Кнопка Войти — больше
                Rectangle {
                    width: parent.width * 0.4
                    height: 64
                    radius: height / 2
                    color: "#998EA8C3"
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Войти")
                        color: "#ECF0F1"
                        font.pixelSize: Theme.fontSizeLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (usernameField.text === "" || passwordField.text === "") {
                                errorLabel.text = qsTr("Заполните все поля")
                            } else {
                                errorLabel.text = ""
                                pageStack.push(Qt.resolvedUrl("RoleSelectionPage.qml"))
                            }
                        }
                    }
                }

                // Кнопка Регистрация — меньше
                Rectangle {
                    width: parent.width * 0.4
                    height: 64
                    radius: height / 2
                    color: "#991F252A"
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Регистрация")
                        color: "#BDC3C7"
                        font.pixelSize: Theme.fontSizeMedium
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("RegisterPage.qml"))
                    }
                }
            }

            // Ошибка (если есть)
            Label {
                id: errorLabel
                text: ""
                color: "red"
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                wrapMode: Text.Wrap
            }
        }
    }
}
