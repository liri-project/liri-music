import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import 'musicId.js' as Global


Item {
    View {
        anchors {
            fill: parent
            margins: Units.dp(32)
        }

        elevation: 1

        ListView{
            id: albumView
            model: allAlbumsModel.model

            anchors.fill: parent

            delegate: ListItem.Subtitled{
                text: model.modelData.title
                visible: true
                subText: model.modelData.artist

                action: Image {

                    source: 'file://' + model.modelData.art
                    anchors.fill: parent
                }


                MouseArea{
                   id: itemMouseArea
                   anchors.fill: parent
                   onClicked: {
                       albumView.visible = false
                       albumDetailView.visible = true
                       albumDetailView.model = model.modelData.getSong
                       currentAlbum.model = model.modelData.getSong
                       Global.mode = allAlbumsModel.model

                }
            }

            }
        }

        ListView{
            id: albumDetailView
            anchors.fill: parent
            model: allAlbums
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
                   id: itemMouseArea2
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


