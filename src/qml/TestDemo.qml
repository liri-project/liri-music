import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import "../js/musicId.js" as Global
import People 1.0

Item {
    View {
        anchors {
            fill: parent
            margins: Units.dp(32)
        }

        elevation: 1

        ListView {
            anchors.fill: parent
            model: People
            delegate: ListItem.Subtitled{
                text: model.name
                subText: model.newname

                MouseArea{
                   id: itemMouseArea
                   anchors.fill: parent
                   onClicked: {

                       var thisName = model.fileName
                       var thisExt = model.fileName.split('.')
                       if(!thisExt[1]){
                            //demo.showError("isDir", thisName, "Close", true)
                           folderModel.folder = folderModel.folder + '/' + model.fileName
                       }else{
                           //demo.showError("isNotDir", thisName, "Close", true)
                           Global.songId = model.index
                           console.log(model.index)
                           Global.currentFolder = folderModel.folder

                           playMusic.source = folderModel.folder + '/' + model.fileName
                           playMusic.play()
                           button.iconName = 'av/pause'

                       }



                   }

                }
            }

       }

        function callFromPython (){
               listModel.append({name: "Animal",shoeSize: 12});
               listModel.append({name: "Dog",shoeSize: 12});
               listModel.append({name: "Cat",shoeSize: 12});
           }

        Column {
            anchors.fill: parent



        }
    }
}
