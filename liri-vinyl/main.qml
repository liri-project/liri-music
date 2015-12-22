import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import QtMultimedia 5.5
import Qt.labs.folderlistmodel 2.1
import "musicId.js" as Global
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtQuick.LocalStorage 2.0

ApplicationWindow {

    function getMusic(){
        musicFolder.initialMusicScan = "Do this";
        console.log(musicFolder.initialMusicScan);

    }

    Timer {
        id:initScan
        interval:2000; running: false; repeat: false
        onTriggered: {
            musicFolder.initialMusicScan = "Do this";
            console.log(musicFolder.initialMusicScan);

        }
    }

    Timer {
        id: setSeekTimer
        interval: 500; running: false; repeat: false
        onTriggered: {
            seeker.maximumValue = parseInt(playMusic.duration)
            if(playMusic.metaData.albumTitle){
                var artist = playMusic.metaData.albumTitle
            }else{
                var artist = 'Unknown Album'
            }
            if(playMusic.metaData.title){
                var title = playMusic.metaData.title
            }else{
                folderModel.folder = Global.currentFolder
                var title = folderModel.get(Global.songId, 'fileName')
            }


        }
    }
    Timer {
        id: durationTimer
        interval: 100; running: false; repeat: true
        onTriggered: {
            if (playMusic.playbackState == 1 && !seeker.pressed){
            var curtime = playMusic.position
            seeker.value = curtime
            }

        }
    }

    Timer {
        id: changeView
        interval: 100; running: false; repeat: false
        onTriggered: {

                example.source = Qt.resolvedUrl("%1Demo.qml").arg(demo.selectedComponent.replace(" ", ""))

        }
    }
    Timer {
        id: myTimer
             interval: 500; running: false; repeat: false
             onTriggered: {

                 if(playMusic.metaData.albumTitle){
                     var artist = playMusic.metaData.albumTitle
                 }else{
                     var artist = 'Unknown Album'
                 }
                     if(playMusic.metaData.title){
                         var title = playMusic.metaData.title
                     }else{
                         folderModel.folder = Global.currentFolder
                         var title = folderModel.get(Global.songId, 'fileName')
                     }




                 }

    }

    Timer {
        id: musicWithFolder
            interval: 100; running: false; repeat: false
            onTriggered: {
                Qt.resolvedUrl("AllMusicDemo.qml")
                selectedComponent = "All Music"
            }
    }

    Timer {
        id: delayedPlay
        interval: 100; running: false; repeat: false
        onTriggered: {
            folderModel.folder = 'file://' + loadedFileFolder
            albumFolder.folder = 'file://' + loadedFileFolder
            playMusic.play()

        }

    }

    Timer {
        id: nextTimer
             interval: 200; running: false; repeat: false
             onTriggered: {
                 if(playMusic.source){
                     folderModel.folder = Global.currentFolder

                     if(Global.songId + 1 == folderModel.count){

                         var folder = folderModel.folder
                         folderModel.folder = Global.currentFolder
                         var currentSong = playMusic.source
                         var nextFile = Global.currentFolder + '/' + folderModel.get(0, 'fileName')
                         playMusic.source = nextFile
                         playMusic.play()
                         Global.songId = 1;

                     }else{
                         var folder = folderModel.folder
                         folderModel.folder = Global.currentFolder
                         var currentSong = playMusic.source
                         var nextFile = Global.currentFolder + '/' + folderModel.get(Global.songId + 1, 'fileName')
                         playMusic.source = nextFile
                         playMusic.play()
                         Global.songId++;
                     }
                 }
         }
    }


    Audio {
        id: playMusic

        onStatusChanged: {
            if (status == MediaPlayer.EndOfMedia) {

                if(Global.mode[0].objectName == "allSongObjects"){
                    if(Global.shuffle){
                        var objects = Global.mode
                        if(Global.playedSongs.length == allSongObjects.length){
                            Global.playedSongs = [];
                        }
                        function getRand() {
                            var rand = Math.floor(Math.random() * parseInt(objects.length))
                            if (Global.playedSongs.indexOf(rand) === -1) {
                                return rand;
                            } else {
                                return getRand();
                            }
                        }

                        var rand = getRand();
                        Global.songId = rand;
                        var newSongId = Global.songId
                        folderModel.folder = Global.currentFolder
                        var currentSong = playMusic.source
                        var nextFile = objects[newSongId].path
                        playMusic.source = "file://" + nextFile
                        playMusic.play()

                        demo.title = objects[newSongId].title
                        songPlaying.text = objects[newSongId].artist + ' - ' + objects[newSongId].title
                        page.title = objects[newSongId].artist + ' - ' + objects[newSongId].title
                        demo.title = objects[newSongId].title
                    }else{
                    if(Global.songId == allSongObjects.length){
                        var folder = folderModel.folder
                        folderModel.folder = Global.currentFolder
                        var currentSong = playMusic.source
                        var nextFile = Global.currentFolder + '/' + folderModel.get(1, 'fileName')
                        playMusic.source = nextFile
                        playMusic.play()
                        Global.songId = 1;

                    }else{
                        var objects = Global.mode
                        if(Global.mode == allSongObjects){
                            var mode = allSongObjects;
                        }

                        if(Global.playedSongs.length == allSongObjects.length){
                            Global.playedSongs = [];
                        }


                        Global.songId = Global.songId + 1;
                        var newSongId = Global.songId
                        var currentSong = playMusic.source
                        var nextFile = allSongObjects[newSongId].path
                        playMusic.source = "file://" + nextFile
                        playMusic.play()

                        demo.title = allSongObjects[newSongId].title
                        songPlaying.text = allSongObjects[newSongId].artist + ' - ' + allSongObjects[newSongId].title
                        page.title = allSongObjects[newSongId].artist + ' - ' + allSongObjects[newSongId].title
                        demo.title = allSongObjects[newSongId].title
                    }
                    }
                }else{
                    if(Global.shuffle){
                        objects = currentAlbum.model
                        if(Global.playedSongs.length == objects.length){
                            Global.playedSongs = [];
                        }
                        function getRand() {
                            var rand = Math.floor(Math.random() * parseInt(objects.length))
                            if (Global.playedSongs.indexOf(rand) === -1) {
                                return rand;
                            } else {
                                return getRand();
                            }
                        }

                        rand = getRand();
                        Global.songId = rand;
                        newSongId = Global.songId
                        folderModel.folder = Global.currentFolder
                        currentSong = playMusic.source
                        nextFile = objects[newSongId].path
                        playMusic.source = "file://" + nextFile
                        playMusic.play()

                        demo.title = objects[newSongId].title
                        songPlaying.text = objects[newSongId].artist + ' - ' + objects[newSongId].title
                        page.title = objects[newSongId].artist + ' - ' + objects[newSongId].title
                        demo.title = objects[newSongId].title
                    }else{
                        objects = currentAlbum.model
                    if(Global.songId == objects.length){
                        var folder = folderModel.folder
                        folderModel.folder = Global.currentFolder
                        var currentSong = playMusic.source
                        var nextFile = Global.currentFolder + '/' + folderModel.get(1, 'fileName')
                        playMusic.source = nextFile
                        playMusic.play()
                        Global.songId = 1;

                    }else{
                        objects = currentAlbum.model

                        if(Global.playedSongs.length == objects.length){
                            Global.playedSongs = [];
                        }


                        Global.songId = Global.songId + 1;
                        newSongId = Global.songId
                        currentSong = playMusic.source
                        nextFile = objects[newSongId].path
                        playMusic.source = "file://" + nextFile
                        playMusic.play()

                        demo.title = objects[newSongId].title
                        songPlaying.text = objects[newSongId].artist + ' - ' + objects[newSongId].title
                        page.title = objects[newSongId].artist + ' - ' + objects[newSongId].title
                        demo.title = objects[newSongId].title
                }
            }
                }
            }
        }
        onSourceChanged: {
            if(Global.prevTrack){

            }else{
                Global.playedSongs.push(Global.songId)
            }
            Global.prevTrack = false
            myTimer.start()
            setSeekTimer.start()
            durationTimer.start()
            playButton1.iconName = 'av/pause'
        }
        Component.onCompleted: {
            if(filePathName){
                playMusic.source = 'file://' + filePathName
                delayedPlay.start()
            }
        }


    }

    FolderListModel {
        id: folderModel
        folder: {
            return "file://" + homeDirectory
        }
        nameFilters: [ "*.mp3", "*.wav" ]
        showDotAndDotDot: false
        showFiles: true
    }

    FolderListModel {
        id: streamFolder
        folder: "file://" + streamDirectory
        nameFilters: [ "*.mp3", "*.wav", "*.ogg", "*.m3u", "*.pls", !"streams" ]
        showDotAndDotDot: false
        showFiles: true
    }

    FolderListModel {
        id: albumFolder
        folder: "file://" + homeDirectory
    }

    FolderListModel {
        id: folderGetImage
        folder: "file://" + homeDirectory
        nameFilters: ["*.png", "*.jpg"]
        showFiles: true
    }

    ListView {
        id: allAlbumsModel
        model: {

            if(allAlbums[0].title != "undefined"){
                return allAlbums
            }
        }
        visible: false
    }

    ListView {
        id: currentAlbum
    }

    id: demo
    title: "Liri Music"
    height: Units.dp(700)
    width: Units.dp(1200)

    Timer {
        id: resetFolders
        interval: 100; running:false; repeat:false
        onTriggered: {
            folderModel.folder = "file://" + homeDirectory
            albumFolder.folder = "file://" + homeDirectory
            streamFolder.folder = "file://" + streamDirectory


        }
    }


    // Necessary when loading the window from C++
    visible: true



    theme {
        primaryColor: {
                var db = LocalStorage.openDatabaseSync("vinylmusic", "1.0", "The Example QML SQL!", 1000000);
                db.transaction(
                    function(tx) {
                        // Create the database if it doesn't already exist
                        tx.executeSql('CREATE TABLE IF NOT EXISTS Settings(id INTEGER PRIMARY KEY AUTOINCREMENT, setting TEXT, value TEXT)');
                        // Show all added greetings
                        var rs = tx.executeSql('SELECT * FROM Settings WHERE setting="primaryColor"');
                        var r = ""

                        if(rs.rows.length > 0){
                            for(var i = 0; i < rs.rows.length; i++) {
                                r += rs.rows.item(i).setting + ", " + rs.rows.item(i).value + "\n"
                            }
                            theme.primaryColor = rs.rows.item(0).value
                        }else{
                            theme.primaryColor = Palette.colors["deepOrange"]["500"]
                        }
                    })
            }



        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    property var sidebar: [
            "Albums", "Artists", "All Music", "Streams"
    ]

    property var basicComponents: [
            "Button", "CheckBox", "Progress Bar", "Radio Button",
            "Slider", "Switch", "TextField"
    ]

    property var compoundComponents: [
            "Bottom Sheet", "Dialog", "Forms", "List Items", "Page Stack", "Time Picker", "Date Picker"
    ]

    property var sections: [ sidebar ]

    property var sectionTitles: [ "Collection"]

    property string selectedComponent: sidebar[0]

    function getNextTrack(){

        if(Global.mode[0].objectName == "allSongObjects"){
            if(Global.shuffle){
                var objects = Global.mode
                if(Global.playedSongs.length == allSongObjects.length){
                    Global.playedSongs = [];
                }
                function getRand() {
                    var rand = Math.floor(Math.random() * parseInt(objects.length))
                    if (Global.playedSongs.indexOf(rand) === -1) {
                        return rand;
                    } else {
                        return getRand();
                    }
                }

                var rand = getRand();
                Global.songId = rand;
                var newSongId = Global.songId
                folderModel.folder = Global.currentFolder
                var currentSong = playMusic.source
                var nextFile = objects[newSongId].path
                playMusic.source = "file://" + nextFile
                playMusic.play()

                demo.title = objects[newSongId].title
                songPlaying.text = objects[newSongId].artist + ' - ' + objects[newSongId].title
                page.title = objects[newSongId].artist + ' - ' + objects[newSongId].title
                demo.title = objects[newSongId].title
            }else{
            if(Global.songId == allSongObjects.length){
                var folder = folderModel.folder
                folderModel.folder = Global.currentFolder
                var currentSong = playMusic.source
                var nextFile = Global.currentFolder + '/' + folderModel.get(1, 'fileName')
                playMusic.source = nextFile
                playMusic.play()
                Global.songId = 1;

            }else{
                var objects = Global.mode
                if(Global.mode == allSongObjects){
                    var mode = allSongObjects;
                }

                if(Global.playedSongs.length == allSongObjects.length){
                    Global.playedSongs = [];
                }


                Global.songId = Global.songId + 1;
                var newSongId = Global.songId
                var currentSong = playMusic.source
                var nextFile = allSongObjects[newSongId].path

                playMusic.source = "file://" + nextFile
                playMusic.play()

                demo.title = allSongObjects[newSongId].title
                songPlaying.text = allSongObjects[newSongId].artist + ' - ' + allSongObjects[newSongId].title
                page.title = allSongObjects[newSongId].artist + ' - ' + allSongObjects[newSongId].title
                demo.title = allSongObjects[newSongId].title
            }
            }
        }else{
            if(Global.shuffle){
                objects = currentAlbum.model
                if(Global.playedSongs.length == objects.length){
                    Global.playedSongs = [];
                }
                function getRand() {
                    var rand = Math.floor(Math.random() * parseInt(objects.length))
                    if (Global.playedSongs.indexOf(rand) === -1) {
                        return rand;
                    } else {
                        return getRand();
                    }
                }

                rand = getRand();
                Global.songId = rand;
                newSongId = Global.songId
                folderModel.folder = Global.currentFolder
                currentSong = playMusic.source
                nextFile = objects[newSongId].path
                playMusic.source = "file://" + nextFile
                playMusic.play()

                demo.title = objects[newSongId].title
                songPlaying.text = objects[newSongId].artist + ' - ' + objects[newSongId].title
                page.title = objects[newSongId].artist + ' - ' + objects[newSongId].title
                demo.title = objects[newSongId].title
            }else{
                objects = currentAlbum.model
            if(Global.songId == objects.length){
                var folder = folderModel.folder
                folderModel.folder = Global.currentFolder
                var currentSong = playMusic.source
                var nextFile = Global.currentFolder + '/' + folderModel.get(1, 'fileName')
                playMusic.source = nextFile
                playMusic.play()
                Global.songId = 1;

            }else{
                objects = currentAlbum.model

                if(Global.playedSongs.length == objects.length){
                    Global.playedSongs = [];
                }


                Global.songId = Global.songId + 1;
                newSongId = Global.songId
                currentSong = playMusic.source
                nextFile = objects[newSongId].path
                playMusic.source = "file://" + nextFile
                playMusic.play()

                demo.title = objects[newSongId].title
                songPlaying.text = objects[newSongId].artist + ' - ' + objects[newSongId].title
                page.title = objects[newSongId].artist + ' - ' + objects[newSongId].title
                demo.title = objects[newSongId].title
        }
    }
        }
    }

    function getPrevTrack(){
        if(Global.mode[0].objectName == "allSongObjects"){
            // Remove last item in songsplayed
            Global.playedSongs.splice(-1,1)
            Global.songId = Global.playedSongs[Global.playedSongs.length - 1];
            if(!Global.playedSongs[Global.playedSongs.length]){
                Global.songId = 0
            }

            // get new song id
            var newSongId = Global.songId;
            var nextFile = allSongObjects[Global.songId].path
            Global.prevTrack = true
            playMusic.source = "file://" + nextFile
            playMusic.play()
            demo.title = allSongObjects[newSongId].title
            songPlaying.text = allSongObjects[newSongId].artist + ' - ' + allSongObjects[newSongId].title
            page.title = allSongObjects[newSongId].artist + ' - ' + allSongObjects[newSongId].title
            demo.title = allSongObjects[newSongId].title
        }else{
            var objects = currentAlbum.model
            Global.playedSongs.splice(-1,1)
            Global.songId = Global.playedSongs[Global.playedSongs.length - 1]
            newSongId = Global.songId
            nextFile = objects[newSongId].path
            Global.prevTrack = true
            playMusic.source = "file://" + nextFile
            playMusic.play()
            demo.title = objects[newSongId].title
            songPlaying.text = objects[newSongId].artist + ' - ' + objects[newSongId].title
            page.title = objects[newSongId].artist + ' - ' + objects[newSongId].title
            demo.title = objects[newSongId].title

        }


    }

    function playTriggerAction(){
        if (playMusic.playbackState == 1){
            playMusic.pause()
            playButton1.iconName = 'av/play_arrow'
        }
        else{
            playMusic.play()
            playButton1.iconName = 'av/pause'
        }
    }

    function volumeUp(){
        var curvol = playMusic.volume
        var newVol = curvol + 0.10
        volumeControl.value = newVol
        playMusic.volume = newVol
    }

    function volumeDown(){
        var curvol = playMusic.volume
        var newVol = curvol - 0.10
        volumeControl.value = newVol
        playMusic.volume = newVol

    }

    function setLoaderSource(source){
        example.setSource(source);
    }

    initialPage: TabbedPage {
        id: page

        title: "Liri Vinyl"


        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4

        //Key Navigation
        Keys.onUpPressed: {
            volumeUp()
        }

        Keys.onDownPressed: {
            volumeDown()
        }

        Keys.onSpacePressed: {
            if(playMusic.source){
                playTriggerAction()
            }
        }


        Keys.onRightPressed: {
            if(playMusic.source){
                folderModel.folder = Global.currentFolder
                getNextTrack()
            }
        }


        Keys.onLeftPressed: {
            if(playMusic.source && Global.songId != 0){
                getPrevTrack()
            }
        }

        actions: [

            Action {
                id: settingsButton
                iconName: "action/settings"
                name: "Settings"
                onTriggered: {
                    settingsDialog.show()
                }
            }




        ]

        backAction: navDrawer.action

        NavigationDrawer {
            id: navDrawer

            enabled: page.width < Units.dp(600)

            Flickable {
                anchors.fill: parent

                contentHeight: Math.max(content.implicitHeight, height)

                Column {
                    id: content
                    anchors.fill: parent

                    Repeater {
                        model: sections

                        delegate: Column {
                            width: parent.width

                            ListItem.Subheader {
                                text: sectionTitles[index]
                            }

                            Repeater {
                                model: modelData
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == demo.selectedComponent
                                    onClicked: {
                                        demo.selectedComponent = modelData
                                        navDrawer.close()

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Repeater {
            model: !navDrawer.enabled ? sections : 0

            delegate: Tab {
                title: sectionTitles[index]

                property string selectedComponent: modelData[0]
                property var section: modelData

                sourceComponent: tabDelegate
            }
        }

        Loader {
            anchors.fill: parent
            sourceComponent: tabDelegate

            property var section: []
            visible: navDrawer.enabled
        }
    }

    Dialog {
        id: settingsDialog
        title: "Settings"
        height:Units.dp(400)
        width:Units.dp(600)
        positiveButtonText: "Save"

        Loader {
            height:Units.dp(400)
            width:Units.dp(600)
            anchors.fill: parent
            //anchors.bottomMargin: Units.dp(100)
            asynchronous: true
            visible: true
            source: Qt.resolvedUrl("SettingsDemo.qml")

        }
        //Component.onCompleted: visible = false
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a folder"
        folder: shortcuts.home
        selectFolder: true
        selectMultiple: false
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            musicFolder.getMusicFolder = fileDialog.fileUrls[0].toString()
            console.log(musicFolder.getMusicFolder)
            fileDialog.close()

        }
        onRejected: {
            console.log("Canceled")
            fileDialog.close()
        }
        Component.onCompleted: visible = false
    }

    Dialog {
        id: colorPicker
        title: "Pick color"

        positiveButtonText: "Done"

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: Units.dp(160)
        }

        Grid {
            columns: 7
            spacing: Units.dp(8)

            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]

                Rectangle {
                    width: Units.dp(30)
                    height: Units.dp(30)
                    radius: Units.dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? Units.dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent

                        onPressed: {
                            switch(selection.selectedIndex) {
                                case 0:
                                    theme.primaryColor = parent.color
                                    var db = LocalStorage.openDatabaseSync("vinylmusic", "1.0", "The Example QML SQL!", 1000000);
                                    db.transaction(
                                        function(tx) {
                                            // Create the database if it doesn't already exist
                                            tx.executeSql('CREATE TABLE IF NOT EXISTS Settings(id INT PRIMARY KEY AUTOINCREMENT, setting TEXT, value TEXT)');

                                            var rs = tx.executeSql('SELECT * FROM Settings WHERE setting="primaryColor"')
                                            if(rs.rows.length > 0){
                                                tx.executeSql('UPDATE Settings SET value="' + theme.primaryColor + '" WHERE id=' + rs.rows.item(0).id);

                                            }else{
                                                tx.executeSql('INSERT INTO Settings VALUES (NULL, ?, ?)',  [ 'primaryColor', theme.primaryColor ]);
                                            }

                                        })
                                    break;
                                case 1:
                                    theme.accentColor = parent.color
                                    break;
                                case 2:
                                    theme.backgroundColor = parent.color
                                    break;
                            }
                        }
                    }
                }
            }
        }

        onRejected: {
            // TODO set default colors again but we currently don't know what that is
        }
    }

    Component {
        id: tabDelegate

        Item {
            Sidebar {
                id: sidebar

                expanded: !navDrawer.enabled

                Column {
                    width: parent.width

                    Repeater {
                        model: section
                        delegate: ListItem.Subtitled {

                            text: modelData
                            selected: modelData == selectedComponent
                            action: IconButton {

                                    iconName: {
                                        if(modelData == 'Albums'){
                                        return 'av/album'
                                        }else if(modelData == 'Artists'){
                                            return 'social/person'
                                        }else if(modelData == 'All Music'){
                                            return 'av/queue_music'
                                        }else if(modelData == 'Streams'){
                                            return 'social/public'
                                        }else if(modelData == 'Settings'){
                                            return 'action/settings'
                                        }

                                    }
                                    anchors.topMargin: Units.dp(20)
                                    height:Units.dp(36)
                                    width:Units.dp(12)
                                    anchors.horizontalCenter: parent.horizontalCenter

                            }

                            height:Units.dp(42)

                            onClicked: {
                                Global.playedSongs = []
                                selectedComponent = modelData
                                folderModel.folder = "file://" + homeDirectory
                                albumFolder.folder = "file://" + homeDirectory


                                if(allAlbums[0].title != "undefined"){
                                   allAlbumsModel.model = allAlbums
                                }
                            }

                        }
                    }
                }
            }
            Flickable {
                id: flickable
                anchors {
                    left: sidebar.right
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
                clip: true
                contentHeight: Math.max(example.implicitHeight + 40, height)
                Loader {
                    id: example
                    anchors.fill: parent
                    anchors.bottomMargin: Units.dp(100)
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component

                    source: {

                        if (navDrawer.enabled) {
                            return Qt.resolvedUrl("%1Demo.qml").arg(demo.selectedComponent.replace(" ", ""))
                        } else {
                            return Qt.resolvedUrl("%1Demo.qml").arg(selectedComponent.replace(" ", ""))
                        }

                    }

                }


                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }


    Rectangle {
        color:'#fff'
        height:Units.dp(100)
        width:parent.width
        anchors.bottom: parent.bottom
        border.width: modelData === "white" ? Units.dp(2) : 0
        border.color: Theme.alpha("#aaa", 0.26)

        Rectangle {
            color:'#fff'
            height:50
        }

        Label {
            id: songPlaying
            text: "Nothing playing"
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height:Units.dp(60)
            anchors.left: seeker.left
            width:Units.dp(100)
            color: Theme.light.textColor
        }

        Slider {
            id: seeker
            width: {
                return parseInt(parent.width - 50)
            }
            anchors.horizontalCenter: parent.horizontalCenter
            height:50
            value: 0
            darkBackground: index == 1
            updateValueWhileDragging: true
            color:theme.primaryColor
            anchors.rightMargin: Units.dp(50)
            anchors.leftMargin:Units.dp(50)
            anchors.bottomMargin:Units.dp(190)

            onValueChanged: {
                if(seeker.pressed){
                    durationTimer.stop()
                    playMusic.pause()
                    var newseek = parseInt(seeker.value * 1)
                    playMusic.seek(newseek)
                    playMusic.play()
                    durationTimer.start()
                }
            }
        }



        Rectangle {

            anchors.bottomMargin: Units.dp(0)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height:Units.dp(60)
            width:Units.dp(210)

            IconButton {
                iconName: 'av/skip_previous'
                height:Units.dp(50)
                width:Units.dp(70)
                id: prevButton
                anchors.left: parent.left
                size: Units.dp(30)
                onClicked: {
                    getPrevTrack()
                }
            }

            IconButton {
                iconName: 'av/play_arrow'
                height:Units.dp(50)
                width:Units.dp(70)
                id: playButton1
                anchors.left: prevButton.right
                size: Units.dp(30)
                onClicked: {
                    playTriggerAction()
                }

            }

            IconButton {
                iconName: 'av/skip_next'
                height:Units.dp(50)
                width:Units.dp(70)
                id: nextButton
                anchors.left: playButton1.right
                size: Units.dp(30)

                onClicked: {
                        getNextTrack()

                }
            }

        }


    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Units.dp(10)
        anchors.right: parent.right
        anchors.rightMargin:Units.dp(30)
        height:Units.dp(40)
        width:Units.dp(150)
        Component.onCompleted: {
            if(filePathName){
                folderModel.folder = loadedFileFolder.toString()
                albumFolder.folder = loadedFileFolder.toString()
                playMusic.source = 'file://' + filePathName
                playMusic.play()
            }
        }

        IconButton {
            id: shuffleButton
            iconName: {
                return 'av/shuffle'
            }
            height:Units.dp(40)
            width:Units.dp(50)
            anchors.topMargin: Units.dp(-50)
            anchors.right: volumeIcon.left

            color: {
                if(Global.shuffle){
                    return theme.primaryColor;
                }else{
                    return Theme.light.textColor
                }
            }
            Component.onCompleted: {
                msg.author = "Nick"
                aa.getAlbums = "New Album"
                console.log(aa.getAlbums);

                Global.mode = allSongObjects
                var db = LocalStorage.openDatabaseSync("vinylmusic", "1.0", "The Example QML SQL!", 1000000);
                db.transaction(
                    function(tx) {
                        // Create the database if it doesn't already exist
                        tx.executeSql('CREATE TABLE IF NOT EXISTS Settings(id INTEGER PRIMARY KEY AUTOINCREMENT, setting TEXT, value TEXT)');
                        // Show all added greetings
                        var rs = tx.executeSql('SELECT * FROM Settings WHERE setting="shuffle"');
                        var r = ""

                        if(rs.rows.length > 0){
                            for(var i = 0; i < rs.rows.length; i++) {
                                r += rs.rows.item(i).setting + ", " + rs.rows.item(i).value + "\n"
                            }
                            console.log(r)
                            console.log(rs.rows.item(0).value)
                            Global.shuffle = rs.rows.item(0).value
                            if( rs.rows.item(0).value){
                                shuffleButton.color = theme.primaryColor;
                            }else{
                                shuffleButton.color = Theme.light.textColor
                            }
                        }else{
                            Global.shuffle = false;
                        }



                    })
            }


            onClicked: {
                Global.shuffle = !Global.shuffle
                if(Global.shuffle){
                    this.color = theme.primaryColor;
                }else{
                    this.color = Theme.light.textColor
                }

                var db = LocalStorage.openDatabaseSync("vinylmusic", "1.0", "The Example QML SQL!", 1000000);
                db.transaction(
                    function(tx) {
                        // Create the database if it doesn't aGlobal.shufflelready exist
                        tx.executeSql('CREATE TABLE IF NOT EXISTS Settings(id INT PRIMARY KEY AUTOINCREMENT, setting TEXT, value TEXT)');

                        var rs = tx.executeSql('SELECT * FROM Settings WHERE setting="shuffle"')
                        if(rs.rows.length > 0){
                            tx.executeSql('UPDATE Settings SET value="' + Global.shuffle + '" WHERE id=' + rs.rows.item(0).id);

                        }else{
                            tx.executeSql('INSERT INTO Settings VALUES (NULL, ?, ?)',  [ 'shuffle', Global.shuffle ]);
                        }

                    })
            }

        }

        IconButton {
            anchors.bottom: parent.bottom
            id: volumeIcon
            iconName: 'av/volume_up'
            height:Units.dp(36)
            width:Units.dp(50)
            anchors.topMargin: Units.dp(-50)
            anchors.rightMargin: Units.dp(60)
            anchors.left: parent.left
            color: index == 0 ? Theme.light.textColor : Theme.dark.textColor
            onClicked: {
                if(volumeControl.value == 0.00){
                    volumeControl.value = 1
                    this.iconName = 'av/volume_up'
                    this.color = Theme.light.textColor

                }else{
                    volumeControl.value = 0.00
                    this.iconName = 'av/volume_off'
                    this.color = theme.primaryColor //Theme.alpha('#f33', .9)
                }
            }
        }


        Slider {
            id: volumeControl
            Layout.alignment: Qt.AlignCenter
            width: Units.dp(100)
            anchors.bottom: parent.bottom
            height:Units.dp(36)
            anchors.right: parent.right
            updateValueWhileDragging: true
            color:theme.primaryColor
            value: 1.0
            onValueChanged: {
                if(this.value == 0.00){
                    volumeIcon.iconName = 'av/volume_off'
                    volumeIcon.color = theme.primaryColor //Theme.alpha('#f33', .9)

                }else if(this.value > 0.00 && this.value <= 0.60){
                    volumeIcon.iconName = 'av/volume_down'
                    volumeIcon.color = Theme.light.textColor
                }else{
                    volumeIcon.iconName = 'av/volume_up'
                    volumeIcon.color = Theme.light.textColor
                }
                playMusic.volume = this.value
            }
            Component.onCompleted: {

                    initScan.start();
            }
        }




    }



}

