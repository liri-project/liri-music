# Vinyl Music
The official repo for Vinyl Music (previously called material music).  The idea was to make a pretty, cross platform music player.

Currently (version 0.1) the default functionality is to determine your home directory, and then scan ~/Music and display the resulting albums/songs from that information.  In the near future I'll be moving to using sqlite3 in order to better manage the music database.

### Dependencies
	Python3:
		Mutagen (https://github.com/LordSputnik/mutagen)

	Qt:
		Qt5.5 (Tested using 5.5.1)
		qml-material (https://github.com/papyros/qml-material)
		pyotherside (https://thp.io/2011/pyotherside)

Material Music is still very early in development, however, if you'd like to give it a try, just install the dependencies, and then run the following from within the material-music directory to launch:

qml ./main.qml

Or you could build and install it by running (from the vinyl-music subdirectory [containing the .pro file):

qmake

make

make install

### Screenshots
Latest version
![Image of Material Music 1]
(https://github.com/nickgermaine/material-music/blob/master/images/screen2.png)

Version 0.00.1
![Image of Material Music]
(https://github.com/nickgermaine/material-music/blob/master/images/screen1.png)
