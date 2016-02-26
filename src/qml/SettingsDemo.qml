import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import "../js/musicId.js" as Global


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

                onClicked: fileDialog.open()

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
                    fileDialog.open()
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
                    checked: {
                        if(Global.shuffle){
                            return true
                        }else{
                            return false
                        }
                    }
                }


                onClicked: {

                    shuffleSwitch.checked = !shuffleSwitch.checked
                    Global.shuffle = !Global.shuffle
                    if(Global.shuffle){
                        shuffleLabel.text = 'On'
                    }else{
                        shuffleLabel.text = 'Off'
                    }
                }

                action: Icon {
                    anchors.centerIn: parent
                    name: "av/shuffle"
                    size: Units.dp(32)
                }
            }
        }

