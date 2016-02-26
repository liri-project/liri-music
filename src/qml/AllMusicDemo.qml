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




        ListView {
            anchors.fill: parent
            model: allSongObjects
            delegate: ListItem.Subtitled{
                text: model.modelData.title
                visible: true
                subText: {
                    if(model.modelData.artist && model.modelData.album){
                        return model.modelData.artist + ' - ' + model.modelData.album
                    }else if(model.modelData.album){
                        return model.modelData.album;
                    }else {
                        return 'Unknown Album'
                    }
                }

                action: Image {

                    source: {
                        if(model.modelData.art != 'placeholder'){
                        return "file://" + model.modelData.art
                        }else{
                            return "qrc:/images/placeholder.png"
                        }
                    }
                    anchors.fill: {
                    return parent
                    }
                }


                MouseArea{
                   id: itemMouseArea2
                   anchors.fill: parent
                   onClicked: {
                       Global.playedSongs.push(model.index)
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
            Global.mode = allSongObjects
        }
    }
}
