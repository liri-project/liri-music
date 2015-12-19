import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: demo

    title: "Liri Music"

    // Necessary when loading the window from C++
    visible: true
    height:Units.dp(500)

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    property var styles: [
            "Icons", "Custom Icons", "Color Palette", "Typography"
    ]

    property var basicComponents: [
            "Button", "CheckBox", "Progress Bar", "Radio Button",
            "Slider", "Switch", "TextField"
    ]

    property var compoundComponents: [
            "Bottom Sheet", "Dialog", "Forms", "List Items", "Page Stack", "Time Picker", "Date Picker"
    ]

    property var sections: [ styles, basicComponents, compoundComponents ]

    property var sectionTitles: [ "Style", "Basic Components", "Compound Components" ]

    property string selectedComponent: styles[0]

    initialPage: TabbedPage {
        id: page

        title: "Liri Music"

        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4



        backAction: navDrawer.action

Rectangle {
    width:Units.dp(400)
    height:Units.dp(100)
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    color:Theme.alpha("#000", 0)



        Text {
            text: "You\'re music will appear here in just a moment."
            height:Units.dp(50)
            anchors.top:progress.bottom
            anchors.topMargin: Units.dp(50)
            font.pixelSize: 14
            color:'#777'
            anchors.horizontalCenter: parent.horizontalCenter
        }


        ProgressBar {
            id: progress
            width:parent.width
            Layout.fillWidth: true
            color: Palette.colors["blue"]["500"]
            indeterminate: true

        }
}
    }


    }

