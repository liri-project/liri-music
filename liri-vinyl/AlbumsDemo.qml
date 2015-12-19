import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import 'musicId.js' as Global


Item {


    Button {
        id: backButton
        text: "Back to All Albums"
        anchors.left: parent.left
        onClicked: {
            albumDetailView.visible = false
            albumView.visible = true
            backButton.visible = false
            mainAlbumContainer.elevation = 0
        }
        anchors.margins: Units.dp(32)
        visible: false
    }



    View {
        id: mainAlbumContainer
        anchors {
            fill: parent
            margins: Units.dp(32)

        }
        visible:true
        elevation: 0

        GridView{
            id: albumView
            model: allAlbumsModel.model


                width: parent.width
                height: parent.height
                cellWidth: 240
                cellHeight: 240
                anchors.centerIn: parent


                Component {
                    id: contactsDelegate


                    View {
                        id: wrapper
                        width: 220
                        height: 220
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

                            height:220
                            width:220
                        }

                        Rectangle {
                            gradient: Gradient {
                                    GradientStop { position: 0.0; color: Theme.alpha("#000", 0.0) }
                                    GradientStop { position: 1.0; color: "#000" }
                                }
                            height:220
                            width:220


                        Text {
                            id: contactInfo
                            text: title
                            color: Theme.dark.textColor
                            font.pixelSize: 12
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: Units.dp(10)
                            wrapMode: Text.WordWrap
                            width: Units.dp(200)
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
                               mainAlbumContainer.elevation = 1

                        }
                    }
                    }
                }

                delegate: contactsDelegate
                focus: true
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


