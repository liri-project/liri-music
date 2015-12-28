# Liri Vinyl
The official repo for Liri Vinyl (previously called Vinyl Music).  The idea is to provide the first cross-platform, material-design-inspired music player, full of awesome features (and all the stuff you'd expect out of a music player).

![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/master/images/lirimusicnew.png)

For current functionality, see the "Features" section below.


### Dependencies
	Basic:
		gstreamer
	c++:
		TagLib (http://taglib.github.io/)
	Qt:
		Qt5.5 (Tested using 5.5.1)
		qml-material (https://github.com/papyros/qml-material)

Liri Vinyl is still very early in development, however, if you'd like to give it a try, just install the dependencies, and then Qt Creator, and open the .pro file.  Then run it from Qt Creator.

Or you could build it by running the following (from the liri-vinyl subdirectory containing the .pro file):

	qmake
	make
	make install


### Features
##### Keyboard Navigation:
	Spacebar = play/pause
	Left Arrow = previous track (if any)
	Right Arrow = next track
	up/down = volume up/down
	
##### Local Music
Just drop your music folders into ~/Music, and Liri Vinyl will find them
	
##### Online Streams
Download online streams in m3u format, and save them to ~/Music/streams

##### Shuffle
This feature is implemented, but can be a bit buggy at times.  The shuffle feature works flawlessy to generate the next track, but when using the previous button, it can sometimes not find the previous track, and so reverts to track 0.

##### Database Integration
Currently the database gets built during the initial opening of the application, but no way to refresh the database from within the app yet.  The built database is then used to generate song lists (All Songs, Albums, and soon Artists)


### Screenshots
Version 0.6
![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen5.png)

Version 0.5
![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen4.png)


Version 0.2: Prior to December 11 (couldn't decide where to put the audio controls)
![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen3.png)

Version 0.1: Prior to volume, online streams, settings, and bottom controls:
![Image of Material Music 1]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen2.png)

Version 0.00.1
![Image of Material Music]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen1.png)
