import QtQuick
import QtQuick.Controls

import QMLUtils

Flickable {
    id: flick

    property url filePath: ""
    property bool isModified: false

    contentWidth: edit.contentWidth
    contentHeight: edit.contentHeight
    clip: true

    function ensureVisible(r)
    {
        if (contentX >= r.x)
            contentX = r.x;
        else if (contentX+width <= r.x+r.width)
            contentX = r.x+r.width-width;
        if (contentY >= r.y)
            contentY = r.y;
        else if (contentY+height <= r.y+r.height)
            contentY = r.y+r.height-height;
    }

    TextEdit {
        id: edit
        width: flick.width
        height: flick.height
        focus: true
        wrapMode: TextEdit.Wrap
        onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
        onTextChanged: flick.isModified = true
    }

    function openFile()
    {
        if (flick.filePath === "")
        {
            console.log("No file selected")
            return
        }

        edit.text = QMLUtils.readTextFile(flick.filePath)
    }
    function saveFile()
    {
        if (flick.filePath === "")
        {
            console.log("No file selected")
            return
        }

        QMLUtils.writeTextFile(flick.filePath, edit.text)
    }
}
