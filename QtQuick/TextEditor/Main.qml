import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QMLUtils

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Text Editor")

    property url filePath: ""

    // open file
    FileDialog {
        id: openFileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        onAccepted: {
            root.filePath = selectedFile
            root.openFile()
        }
    }
    // save file
    FileDialog {
        id: saveFileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        onAccepted: {
            root.filePath = selectedFile
            root.saveFile()
        }
    }
    // save as file
    FileDialog {
        id: saveAsFileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        onAccepted: {
            root.filePath = selectedFile
            root.saveAsFile()
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&New..."); onTriggered: root.newFile() }
            Action { text: qsTr("&Open..."); onTriggered: openFileDialog.open() }
            Action { text: qsTr("&Save"); onTriggered: root.saveFile() }
            Action { text: qsTr("Save &As..."); onTriggered: saveAsFileDialog.open() }
            MenuSeparator { }
            Action { text: qsTr("&Close"); onTriggered: root.closeFile() }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("&Undo"); onTriggered: root.undo() }
            Action { text: qsTr("&Redo"); onTriggered: root.redo() }
            MenuSeparator { }
            Action { text: qsTr("&Find"); onTriggered: root.find() }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: bar
            Layout.fillWidth: true

            Repeater
            {
                model: ObjectModel
                {
                    id: barContainer
                }
            }
        }

        StackLayout {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: bar.currentIndex

            Repeater
            {
                model: ObjectModel
                {
                    id: stacContainer
                }
            }
        }
    }

    Component {
        id: tabButtonComponent
        TabButton { }
    }

    Component {
        id: fileTextEditComponent
        FileTextEdit { }
    }

    function openFile()
    {
        var fileTextEdit = fileTextEditComponent.createObject(stack, { filePath: root.filePath })
        fileTextEdit.openFile()
        stacContainer.append(fileTextEdit)
        var tab = tabButtonComponent.createObject(bar, { text: QMLUtils.getFileName(root.filePath) })
        barContainer.append(tab)
    }

    function saveFile()
    {
        if (bar.currentIndex === -1)
        {
            console.log("No file selected")
            return
        }
        var fileTextEdit = stacContainer.get(bar.currentIndex)
        if (fileTextEdit.filePath === "")
        {
            saveFileDialog.open()
            return
        }
        fileTextEdit.saveFile()
    }

    function saveAsFile()
    {
        if (bar.currentIndex === -1)
        {
            console.log("No file selected")
            return
        }
        var fileTextEdit = stacContainer.get(bar.currentIndex)
        fileTextEdit.filePath = root.filePath
        fileTextEdit.saveFile()
    }

    function closeFile()
    {
        if (bar.currentIndex === -1)
        {
            console.log("No file selected")
            return
        }
        var fileTextEdit = stacContainer.get(bar.currentIndex)
        if (fileTextEdit.isModified)
        {
            var dialog = Dialog
            {
                title: "Save changes"
                standardButtons: Dialog.Yes | Dialog.No | Dialog.Cancel
                onAccepted: saveFile(); barContainer.remove(bar.currentIndex); stacContainer.remove(bar.currentIndex)
                onRejected: barContainer.remove(bar.currentIndex); stacContainer.remove(bar.currentIndex)
            }
            dialog.open()
        }
    }

    function newFile()
    {
        var fileTextEdit = fileTextEditComponent.createObject(stack, { filePath: "" })
        stacContainer.append(fileTextEdit)
        var tab = tabButtonComponent.createObject(bar, { text: "New File" })
        barContainer.append(tab)
    }

    function undo()
    {
        if (bar.currentIndex === -1)
        {
            console.log("No file selected")
            return
        }
        var fileTextEdit = stacContainer.get(bar.currentIndex)
        fileTextEdit.edit.undo()
    }

    function redo()
    {
        if (bar.currentIndex === -1)
        {
            console.log("No file selected")
            return
        }
        var fileTextEdit = stacContainer.get(bar.currentIndex)
        fileTextEdit.edit.redo()
    }

    function find()
    {
        if (bar.currentIndex === -1)
        {
            console.log("No file selected")
            return
        }
        var fileTextEdit = stacContainer.get(bar.currentIndex)
        // find
    }
}
