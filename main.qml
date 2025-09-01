import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

ApplicationWindow {
    minimumWidth: mainView.columnWidthProvider(0) * 11.3
    minimumHeight: mainView.rowHeightProvider(0) * 12.3
    maximumWidth: mainView.columnWidthProvider(0) * 11.3
    maximumHeight: mainView.rowHeightProvider(0) * 12.3
    width: mainView.columnWidthProvider(0) * 11.3
    height: mainView.rowHeightProvider(0) * 12.3
    visible: true
    title: "Sumplete"

    MessageDialog {
        id : winGameMsg
        title: "Начать новую игру?"
        icon: StandardIcon.Question
        text: "УРА! Вы победили! Начать новую игру?"
        informativeText: "Поле сформируется с нуля. Все текущие пометки будут утеряны. В статистике засчитается как выигрыш."

        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: mainView.model.newGame()
    }

    MessageDialog {
        id : newGameMsg
        title: "Начать новую игру?"
        icon: StandardIcon.Question
        text: "Начать новую игру?"
        informativeText: "Поле сформируется с нуля. Все текущие пометки будут утеряны. В статистике засчитается как проигрыш."

        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: mainView.model.newGame()
    }

    MessageDialog {
        id : resetGameMsg
        title: "Начать текущую игру заново?"
        icon: StandardIcon.Question
        text: "Начать текущую игру заново?"
        informativeText: "Поле останется прежним. Все текущие пометки будут утеряны. В статистике засчитается как проигрыш."

        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: mainView.model.resetGame()
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        rows: 3
        columns: 2

        RowLayout {
            spacing: 3
            Layout.row: 0
            Layout.column: 0
            Layout.columnSpan: 2

            Button {
                Layout.rightMargin: 25
                text: "Новая игра"
                onClicked:
                    newGameMsg.open()
            }

            Button {
                text: "Начать заново"
                onClicked:
                    resetGameMsg.open()
            }

            Button {
                text: "Подсказка"
                onClicked:
                    tableModel.makeHint()
            }
        }

        TableView {
            id: mainView
            Layout.row: 1
            Layout.column: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            columnSpacing: 1
            rowSpacing: 1


            model: tableModel

            rowHeightProvider: function(row) { return 40; }  // Можно динамически
            columnWidthProvider: function(width) { return 40; }  // Можно динамически

            delegate:
                Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.color: "black"
                border.width: 1
                color: "white"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        tableModel.toggleMark(model.index)
                    }
                    Image {
                        anchors.fill: parent
                        id: img
                        source: model.image
                        Text {
                            anchors.centerIn: parent
                            text: model.value
                            font.pixelSize: 20
                        }
                    }
                }
            }
        }


        // Столбец сумм
        ColumnLayout {
            Layout.alignment: "AlignTop"
            Layout.row: 1
            Layout.column: 1
            spacing: 1

            Repeater {
                model: tableModel.rowSums

                Rectangle {
                    Layout.preferredWidth: mainView.columnWidthProvider(0)
                    Layout.preferredHeight: mainView.rowHeightProvider(0)
                    border.color: "black"
                    border.width: 1
                    color: "#f0f0f0"

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: 20
                    }
                }
            }
        }

        // Столбец сумм
        RowLayout {
            spacing: 1
            Layout.row: 2
            Layout.column: 0

            Repeater {
                model: tableModel.colSums

                Rectangle {
                    Layout.preferredWidth: mainView.columnWidthProvider(0)
                    Layout.preferredHeight: mainView.rowHeightProvider(0)

                    border.color: "black"
                    border.width: 1
                    color: "#f0f0f0"

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: 20
                    }
                }
            }
        }

        Connections {
            target: mainView.model
            function onSignal_winGame() {
                winGameMsg.open();
            }
        }

        Component.onCompleted: {
            setX(screen.virtualX + screen.width / 2 - width / 2);
            setY(screen.virtualY + screen.height / 2 - height / 2);
        }
    }
}
