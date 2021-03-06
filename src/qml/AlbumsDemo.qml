import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import "../js/musicId.js" as Global


Item {


    Button {
        id: backButton
        text: "Back to All Albums"
        anchors.left: parent.left
        onClicked: {
            albumDetailView.visible = false
            albumView.visible = true
            backButton.visible = false
            mainAlbumContainer.visible = false
            page.title = "Liri Music"
        }
        anchors.margins: Units.dp(32)
        visible: false
    }

    GridView{
        id: albumView
        model: {
            if(allAlbumsModel){
            return allAlbumsModel.model
            }
        }
        anchors {
            fill: parent
            margins: Units.dp(32)

        }


        width: parent.width - Units.dp(64)
        height: parent.height
        cellWidth: 180
        cellHeight: 180
        anchors.centerIn: parent


        Component {
            id: albumsDelegate


            View {
                id: wrapper
                width: 160
                height: 160
                anchors.margins:Units.dp(32)
                elevation: 1


                Image {
                    id: albumImage
                    source: {
                        if(art != 'placeholder'){
                        return "file://" + art
                        }else{
                            return "qrc:/images/placeholder.png"
                        }
                    }

                    height:160
                    width:160
                }

                Rectangle {
                    gradient: Gradient {
                            GradientStop { position: 0.0; color: Theme.alpha("#000", 0.0) }
                            GradientStop { position: 1.0; color: "#000" }
                        }
                    height:160
                    width:160


                Text {
                    id: contactInfo
                    text: title
                    color: Theme.dark.textColor
                    font.pixelSize: 12
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: Units.dp(10)
                    wrapMode: Text.WordWrap
                    width: Units.dp(140)
                    verticalAlignment: Text.AlignBottom
                    height:Units.dp(50)
                    anchors.margins:Units.dp(10)

                }
                }

                MouseArea{
                   id: itemMouseArea
                   anchors.fill: parent
                   onClicked: {
                       albumView.visible = false
                       albumDetailView.visible = true
                       albumDetailView.model = getSong
                       currentAlbum.model = getSong
                       Global.mode = allAlbumsModel.model
                       backButton.visible = true
                       mainAlbumContainer.visible = true
                       page.title = title

                }
            }
            }
        }

        delegate: albumsDelegate
        focus: true
    }



    View {
        id: mainAlbumContainer
        anchors {
            fill: parent
            margins: Units.dp(32)

        }
        visible:false
        elevation: 1






        ListView{
            id: albumDetailView
            anchors.fill: parent
            model: allAlbums
            visible: false
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
                    anchors.fill: parent
                }


                MouseArea{
                   id: itemMouseArea2
                   anchors.fill: parent
                   onClicked: {
                       playMusic.source = "file://" + model.modelData.path
                       playMusic.play()
                       songPlaying.text = model.modelData.artist + ' - ' + model.modelData.title
                       page.title = model.modelData.artist + ' - ' + model.modelData.title


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


