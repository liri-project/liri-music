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

        ListView {
            anchors.fill: parent
            model: folderModel
            delegate: ListItem.Subtitled{

                text: model.fileName
                visible: {
                    console.log(model.name)
                    if(model.fileName != "streams"){

                        return true;
                    }else{
                        this.height = Units.dp(0)
                        return false;
                    }
                }
                subText: {
                    var thisName = model.fileName
                    var thisExt = model.fileName.split('.')
                    if(!thisExt[1]){
                        return model.fileName
                    }else{
                        var curdir = Qt.resolvedUrl(folderModel.folder).split('/')
                        return curdir[curdir.length - 1]
                    }
                }

                action: Image {

                    source: {
                        Global.currentFolder = folderModel.folder

                        var thisName = model.fileName
                        var thisExt = model.fileName.split('.')
                        if(!thisExt[1]){
                            Global.currentFolder = folderModel.folder
                            albumFolder.folder = Global.currentFolder
                            folderGetImage.folder = albumFolder.folder + model.fileName
                            var thisImage = Qt.resolvedUrl(albumFolder.folder + '/' + model.fileName + "/AlbumArtSmall.jpg")

                            if(thisImage){
                                return thisImage
                            }else{
                                return 'av/volume_up'
                            }
                        }else{
                            var thisImage = Qt.resolvedUrl(folderModel.folder + "/AlbumArtSmall.jpg")
                            if(thisImage){
                                return thisImage
                            }else{
                                return 'av/volume_up'
                            }

                        }
                    }
                    anchors.fill: parent
                }


                MouseArea{
                   id: itemMouseArea
                   anchors.fill: parent
                   onClicked: {
                       Global.currentFolder = folderModel.folder
                       var thisName = model.fileName
                       var thisExt = model.fileName.split('.')
                       if(!thisExt[1]){
                            //demo.showError("isDir", thisName, "Close", true)
                           folderModel.folder = folderModel.folder + '/' + model.fileName
                       }else{
                           //demo.showError("isNotDir", thisName, "Close", true)
                           Global.songId = model.index
                           Global.currentFolder = folderModel.folder
                           albumFolder.folder = Global.currentFolder

                           playMusic.source = folderModel.folder + '/' + model.fileName
                           playMusic.play()
                           playButton1.iconName = 'av/pause'

                       }

                   }

                }
            }

        }

        Column {
            anchors.fill: parent
        }
    }
}
