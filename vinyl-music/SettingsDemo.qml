import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Item {
    View {
        anchors {
            fill: parent
            margins: Units.dp(32)
        }

        elevation: 1

        Column {
            anchors.fill: parent

            ListItem.Subtitled {
                text: "Theme"
                subText: "Select color theme"
                onClicked: colorPicker.show()

                action: Icon {
                    anchors.centerIn: parent
                    name: "image/color_lens"
                    size: Units.dp(32)
                }
            }

            ListItem.Subtitled {
                text: "Music Folders"
                subText: "Add/Remove music folders"

                onClicked: colorPicker.show()

                action: Icon {
                    anchors.centerIn: parent
                    name: "content/sort"
                    size: Units.dp(32)
                }
            }

            ListItem.Subtitled {
                text: "Online Streams"
                subText: "Manage online streams"

                onClicked: {
                    playMusic.source = 'file:///home/nick/Downloads/tronic.ogg.m3u'
                    playMusic.play()
                }

                action: Icon {
                    anchors.centerIn: parent
                    name: "social/public"
                    size: Units.dp(32)
                }
            }

            ListItem.Subtitled {
                text: "Shuffle Music"
                subText: "Randomize your song order"
                secondaryItem: Switch {
                    id: shuffleSwitch
                    anchors.verticalCenter: parent.verticalCenter
                }

                onClicked: shuffleSwitch.checked = !shuffleSwitch.checked

                action: Icon {
                    anchors.centerIn: parent
                    name: "av/shuffle"
                    size: Units.dp(32)
                }
            }
        }
    }
}
