import QtQuick 2.6
import Sailfish.Silica 1.0

Page {
    id: registerPage

    property string username: ""
    property string password: ""

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
            spacing: Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.3

            Label {
                text: qsTr("Регистрация")
                font.pixelSize: Theme.fontSizeExtraLarge
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                color: "#ECF0F1"
            }

            Item {
                width: 1
                height: Theme.paddingSmall * 1.5  // или любое другое подходящее значение
            }


            Column {
                width: parent.width
                spacing: Theme.paddingSmall * 0.8
                anchors.horizontalCenter: parent.horizontalCenter

                TextField {
                    id: usernameField
                    width: parent.width * 0.8
                    placeholderText: qsTr("Имя пользователя")
                    color: "#ECF0F1"
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignLeft
                    anchors.horizontalCenter: parent.horizontalCenter
                    onTextChanged: username = text
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
                    onTextChanged: password = text
                }
            }

            Item {
                width: 1
                height: Theme.paddingLarge * 1
            }

            Column {
                width: parent.width
                spacing: 40
                anchors.horizontalCenter: parent.horizontalCenter

                // Кнопка Зарегистрироваться
                Rectangle {
                    width: parent.width * 0.6
                    height: 64
                    radius: height / 2
                    color: "#998EA8C3"
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Зарегистрироваться")
                        color: "#ECF0F1"
                        font.pixelSize: Theme.fontSizeLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (username === "" || password === "") {
                                errorLabel.text = qsTr("Пожалуйста, заполните все поля")
                            } else {
                                errorLabel.text = qsTr("Регистрация выполнена (эмуляция)")
                                console.log("Регистрация:", username, password)
                                // Тут можно вызвать C++ метод регистрации
                            }
                        }
                    }
                }

                // Кнопка Назад ко входу
                Rectangle {
                    width: parent.width * 0.4
                    height: 64
                    radius: height / 2
                    color: "#991F252A"
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Назад ко входу")
                        color: "#BDC3C7"
                        font.pixelSize: Theme.fontSizeMedium
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.pop()
                    }
                }
            }

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
