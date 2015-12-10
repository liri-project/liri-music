import QtQuick 2.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import QtMultimedia 5.5
import Qt.labs.folderlistmodel 2.1
import "musicId.js" as Global
import QtQuick.Layouts 1.1

ApplicationWindow {
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
            songPlaying.text = artist + ' - ' + title

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


                     page.title = artist + ' - ' + title
                     demo.title = title

                     if(loadedFileFolder){
                         resetFolders.start()
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
                folderModel.folder = Global.currentFolder
               console.log('length of tracks: ', folderModel.count)
               if(Global.playedSongs.length == folderModel.count){
                   Global.playedSongs = [];
               }

                if(Global.shuffle){
                    function getRand() {
                        var rand = Math.floor(Math.random() * folderModel.count)
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
                    var nextFile = Global.currentFolder + '/' + folderModel.get(newSongId, 'fileName')
                    playMusic.source = nextFile
                    playMusic.play()


                    console.log(Global.playedSongs)
                    folderModel.folder = Global.currentFolder
                }else{
                    if(Global.songId + 1 == folderModel.count){
                        Global.playedSongs = [];

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
        onSourceChanged: {
            Global.playedSongs.push(Global.songId + 1)

            console.log(Global.playedSongs)
            folderModel.folder = Global.currentFolder
            console.log('This song id is ', folderModel.get(Global.songId, 'fileName'))
            //demo.title = playMusic.metaData.title
            myTimer.start()
            setSeekTimer.start()
            durationTimer.start()

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
            console.log("Home dir is ", homeDirectory);
            return "file://" + homeDirectory
        }
        nameFilters: [ "*.mp3", "*.wav" ]
        showDotAndDotDot: false
        showFiles: true
    }

    FolderListModel {
        id: streamFolder
        folder: "file://" + streamDirectory
        nameFilters: [ "*.mp3", "*.wav", "*.ogg", "*.m3u", "*.pls" ]
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

    id: demo
    title: "Vinyl Music Player"
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
        primaryColor: Palette.colors["deepOrange"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }


    property var sidebar: [
            "All Music", "Albums", "Artists", "Streams", "Settings"
    ]

    property var basicComponents: [
            "Button", "CheckBox", "Progress Bar", "Radio Button",
            "Slider", "Switch", "TextField"
    ]

    property var compoundComponents: [
            "Bottom Sheet", "Dialog", "Forms", "List Items", "Page Stack", "Time Picker", "Date Picker"
    ]

    property var sections: [ sidebar ]

    property var sectionTitles: [ "Collection" ]

    property string selectedComponent: sidebar[0]

    function getNextTrack(){
        if(Global.shuffle){
             folderModel.folder = Global.currentFolder
            console.log('length of tracks: ', folderModel.count)
            if(Global.playedSongs.length == folderModel.count){
                Global.playedSongs = [];
            }

            function getRand() {

                var rand = Math.floor(Math.random() * folderModel.count)
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
            var nextFile = Global.currentFolder + '/' + folderModel.get(newSongId, 'fileName')
            playMusic.source = nextFile
            playMusic.play()


            console.log(Global.playedSongs)
            folderModel.folder = Global.currentFolder
        }else{
        if(Global.songId == folderModel.count){
            var folder = folderModel.folder
            folderModel.folder = Global.currentFolder
            var currentSong = playMusic.source
            var nextFile = Global.currentFolder + '/' + folderModel.get(1, 'fileName')
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

    function getPrevTrack(){

        var folder = folderModel.folder
        folderModel.folder = Global.currentFolder
        var currentSong = playMusic.source
        var nextFile = Global.currentFolder + '/' + folderModel.get(parseInt(Global.playedSongs[-1]), 'fileName')
        playMusic.source = nextFile
        playMusic.play()
        Global.songId = Global.playedSongs[-1];

    }

    function playTriggerAction(){
        if (playMusic.playbackState == 1){
            playMusic.pause()
            button.iconName = 'av/play_arrow'
            button.name = "Play"
            playButton1.iconName = 'av/play_arrow'
        }
        else{
            playMusic.play()
            button.iconName = 'av/pause'
            playButton1.iconName = 'av/pause'
            button.name = "Pause"
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

    initialPage: TabbedPage {
        id: page

        title: "Vinyl Music Player"

        Rectangle {
            height:200
            width:parent
            color: theme.primaryColor

        }

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
                id: playPrev
                iconName: "av/skip_previous"
                name: "Previous"
                onTriggered: {
                    getPrevTrack()

                }

            },

            Action {

                id: button
                iconName: "av/play_circle_filled"
                name: "Play"
                onTriggered: {
                    //showError("Is paused?", "this is paused? " + playMusic.playbackState, "Close", true)
                    playTriggerAction()
                }
            },

            Action {
                id: playNext
                iconName: "av/skip_next"
                name: "Next"
                onTriggered: {// This is available in all editors
                    getNextTrack()

                }

            },

            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: {
                    console.log("Settings")
                    demo.selectedComponent = "Settings"
                    example.source = Qt.resolvedUrl("qrc:/SettingsDemo.qml")
                }
            },
            Action {
                id: shuffleTrigger
                iconName: 'av/shuffle'
                name: 'Shuffle Music'
            },

            Action {
                iconName: "alert/warning"
                name: "THIS SHOULD BE HIDDEN!"
                visible: false
            },

            Action {
                iconName: "action/language"
                name: "Language"
                enabled: false
            },

            Action {
                iconName: "action/account_circle"
                name: "Accounts"
            },
            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
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
                        delegate: ListItem.Standard {
                            text: modelData
                            selected: modelData == selectedComponent
                            onClicked: {
                                console.log(modelData)
                                selectedComponent = modelData
                                folderModel.folder = "file://" + homeDirectory
                                albumFolder.folder = "file://" + homeDirectory

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
            color: index == 0 ? Theme.light.textColor : Theme.dark.textColor
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
            anchors.bottom:Units.dp(190)

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
            anchors.right: shuffleLabel.left
            color: index == 0 ? Theme.light.textColor : Theme.dark.textColor
            onClicked: {
                Global.shuffle = !Global.shuffle
                if(Global.shuffle){
                    shuffleLabel.text = 'On'
                }else{
                    shuffleLabel.text = 'Off'
                }
            }

        }

        Label {
            height:Units.dp(80)
            width:Units.dp(30)
            anchors.topMargin: Units.dp(7)
            id: shuffleLabel
            anchors.right: volumeIcon.left
            anchors.top: volumeIcon.top

            text: {
                if(Global.shuffle){
                    return "On"
                }else{
                    return "Off"
                }

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
            darkBackground: index == 1
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
        }



    }





}
