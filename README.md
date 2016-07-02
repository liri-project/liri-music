# Liri Music
The official repo for Liri Music (previously called Vinyl Music).  The idea is to provide the first cross-platform, material-design-inspired music player, full of awesome features (and all the stuff you'd expect out of a music player).

![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/develop/images/liriworking.png)

### Dependencies
	Basic:
		gstreamer
	c++:
		TagLib (http://taglib.github.io/)
	Qt:
		Qt5.7
		qml-material (https://github.com/papyros/qml-material)

We don't have binaries yet, but feel free to try it out by building from source.  Once all of the requirements are met, clone this repository, update the submodules, and then run the following (from the root of the project):

	mkdir build && cd build
	cmake ../
	cd src
	make
	./liri-vinyl


### Features
##### Keyboard Navigation:
	Spacebar = play/pause
	Left Arrow = previous track (if any)
	Right Arrow = next track
	up/down = volume up/down
	
##### Local Music
Just drop your music folders into ~/Music, and we'll find them
	
##### Online Streams
Download online streams in m3u format, and save them to ~/Music/streams

##### Shuffle
Shuffle got broken recently due to changes.  It should be working again soon.

##### Database Integration
Currently the database gets built during the initial opening of the application, but no way to refresh the database from within the app yet.  The built database is then used to generate song lists (All Songs, Albums, and Artists)


### Screenshots
#### A look back on the dev process

![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen5.png)


![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen4.png)


Prior to December 11 (couldn't decide where to put the audio controls)
![Image of liri vinyl]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen3.png)

Prior to volume, online streams, settings, and bottom controls:
![Image of Material Music 1]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen2.png)

Version 0.00.1
![Image of Material Music]
(https://github.com/liri-project/liri-vinyl/blob/master/images/screen1.png)
