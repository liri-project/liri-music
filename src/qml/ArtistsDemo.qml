import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import "../js/musicId.js" as Global


Item {
    View {
        anchors {
            fill: parent
            margins: Units.dp(32)
        }

        elevation: 1

        ListView{
            id: artistView
            model: allArtists

            anchors.fill: parent

            delegate: ListItem.Subtitled{
                text: model.modelData.artist
                visible: true
                subText: null // model.modelData.artist

                action: Icon {
                    name: "social/person"
                    height:Units.dp(32)
                    width:Units.dp(32)
                    anchors.centerIn: parent

                }


                MouseArea{
                   id: itemMouseArea
                   anchors.fill: parent
                   onClicked: {
                       artistView.visible = false
                       artistDetailView.visible = true
                       artistDetailView.model = model.modelData.getSong
                       //currentArtist.model = model.modelData.getSong
                       Global.mode = allArtistsModel.model

                }
            }

            }
        }

        ListView{
            id: artistDetailView
            anchors.fill: parent
            model: allArtists
            visible: false
            delegate: ListItem.Subtitled{
                text: model.modelData.title
                visible: true
                subText: model.modelData.artist

                action: Image {

                    source: 'file://' + model.modelData.art
                    anchors.fill: parent
                }


                MouseArea{
                   id: itemMouseArea3
                   anchors.fill: parent
                   onClicked: {

                       demo.title = model.modelData.title
                       playMusic.source = "file://" + model.modelData.path
                       playMusic.play()
                       songPlaying.text = model.modelData.artist + ' - ' + model.modelData.title
                       page.title = model.modelData.artist + ' - ' + model.modelData.title
                       demo.title = model.modelData.title

                }
            }

            }
        }


        Column {
            anchors.fill: parent
        }

        Component.onCompleted: {


        }


}

}


