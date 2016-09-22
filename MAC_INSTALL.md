
# macOS Install Instructions
### Overly convoluted version
#### Less ridiculously complicated version coming soon.

### Notes on installation process

${LIRI_MUSIC_ROOT} = /path/to/liri-music
For example, I was in ~/Projects/liri-music when I ran git init, and initialized the git repo there, so my liri root would be ~/Projects/liri-music

macOS ships with old *nix tools.  So we need to update, specifically, gcc.  This guide assumes that you half-know what you’re doing in terminal, and you have home-brew installed.


### Install necessary tools

#### From Homebrew

    brew install gcc gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad gst-plugins-ugly cmake boost

#### GStreamer

Download: [link](https://gstreamer.freedesktop.org/data/pkg/osx/1.8.3/gstreamer-1.0-1.8.3-x86_64-packages.dmg "https://gstreamer.freedesktop.org/data/pkg/osx/1.8.3/gstreamer-1.0-1.8.3-x86_64-packages.dmg")

Install all .pkg files (because I don’t have time right now to go through and see which ones we actually need).


#### Qt5GStreamer

Download: [link](https://gstreamer.freedesktop.org/src/qt-gstreamer/qt-gstreamer-1.2.0.tar.xz "https://gstreamer.freedesktop.org/src/qt-gstreamer/qt-gstreamer-1.2.0.tar.xz")

    cd ~/Downloads
    tar -xvf qt-gstreamer-1.2.0.tar.xz
    mkdir build && cd build
    cmake .. -DCMAKE_PREFIX_PATH=/Users/nickgermaine/Qt/5.7/clang_64/lib/cmake -DQT_VERSION=5 -DCMAKE_CXX_COMPILER=/usr/local/Cellar/gcc/6.2.0/bin/g++-6
    make
    make install


#### Python2
We need to do some stuff with python now.  macOS ships with python 2.7 and the python2 command won't work outside of the bin directory.  So...
    
    alias python2="python2.7"
    curl https://bootstrap.pypa.io/ez_setup.py -o - | sudo python
    sudo easy_install pip
    pip install pyyaml


#### Liri Music
Go back into /path/to/liri-music

    rm -rf build
    mkdir build && cd build
    cmake ../ -DQt5_DIR=/path/to/Qt/5.7/clang_64/lib/cmake/Qt5 -DQt5GStreamer_DIR=/path/to/usr/local/lib/cmake/Qt5GStreamer

    make
