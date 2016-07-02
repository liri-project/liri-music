import QtQuick 2.4
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import "../js/musicId.js" as Global


Item {
    View {
        anchors {
            fill: parent
            margins: dp(32)
        }

        elevation: 1




        ListView {
            anchors.fill: parent
            model: allSongsModel.model //songModel.getAllSongs()
            delegate: ListItem.Subtitled{
                text: model.modelData.title
                visible: true
                subText: {
                    var artist = songModel.getArtist(model.modelData.artist)
                    var album = songModel.getAlbum(model.modelData.album)
                    if(artist && album){
                        return artist + ' - ' + album
                    }else if(album){
                        return album;
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
                       var id = model.modelData

                       Global.songId = model.index
                       Global.currentAlbum = currentAlbum
                       console.log("index? ", model.index)
                       Global.mode = "all songs"
                       var album = songModel.getAlbum(id.album)
                       var artist = songModel.getArtist(id.artist)


                       demo.title = model.modelData.title
                       playMusic.source = "file://" + model.modelData.path
                       playMusic.play()
                       songPlaying.text = album + ' - ' + model.modelData.title
                       page.title = album + ' - ' + model.modelData.title
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
