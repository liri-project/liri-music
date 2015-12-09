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
            model: streamFolder
            delegate: ListItem.Subtitled{
                text: model.fileName
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
                        var thisName = model.fileName
                        var thisExt = model.fileName.split('.')
                        if(!thisExt[1]){
                            return Qt.resolvedUrl(albumFolder.folder + '/' + model.fileName + "/AlbumArtSmall.jpg")
                        }else{
                            return Qt.resolvedUrl(folderModel.folder + "/AlbumArtSmall.jpg")

                        }
                    }
                    anchors.fill: parent
                }


                MouseArea{
                   id: itemMouseArea
                   anchors.fill: parent
                   onClicked: {

                       var thisName = model.fileName
                       var thisExt = model.fileName.split('.')
                       if(!thisExt[1]){
                            //demo.showError("isDir", thisName, "Close", true)
                           streamFolder.folder = folderModel.folder + '/' + model.fileName
                       }else{
                           //demo.showError("isNotDir", thisName, "Close", true)
                           playMusic.source = streamFolder.folder + '/' + model.fileName
                           playMusic.play()
                           button.iconName = 'av/pause'

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
