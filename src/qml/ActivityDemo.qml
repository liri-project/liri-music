import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import "../js/musicId.js" as Global
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtWebEngine 1.1


Column {
    anchors.fill: parent
    id: browserContainer

    Component {
        id: ctn
        WebEngineView {
            id: webView
            anchors.fill: parent
            url: "http://pointybracket.net/"
            focus: true

        }

    }
}
