# Harvest-Rogue

[![Gitter](https://badges.gitter.im/essial/harvest-rogue.svg)](https://gitter.im/essial/harvest-rogue?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Build Status](https://travis-ci.org/essial/harvest-rogue.svg?branch=master)](https://travis-ci.org/essial/harvest-rogue)

![Title](http://i.imgur.com/IfmnANS.png) 

Welcome to the project page for Harvest-Rogue, a Harvest Moon-inspired roguelike! This project is very early in development, so check back often for updates!

While testing the game, pressing F12 at any time will immediately quit the game. This will remain in place during development.

## Tasks
The following tasks are currently being worked on:

* General Features
    - Statistics Window Area
    - Basic map filed
    - Basic movement controls


## Compiling

### Linux

* Install <code>build-essential</code>, <code>cmake</code>, and <code>libncurses-dev</code>, and <code>autotools</code> packages for your host environment.
* Clone the source to your home folder with <code>git clone git@github.com:essial/harvest-rogue.git</code>
* Create and navigate to an empty output folder like <code>[projectroot]/output/</code>.
* Generate the autotools build scripts by running <code>$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr/bin</code>
* Now you may run **make** to build the project. You can run it locally with <code>$ ./harvest-rogue</code>, or install it with <code># make install</code>

### Mac OSX

The build process is essentially the same as for Linux, especially if you use [Brew](http://brew.sh/):

```
~/projects/harvest-rogue$ brew install gcc make cmake ncurses
~/projects/harvest-rogue$ cmake .
~/projects/harvest-rogue$ make
```

### Windows

There is currently no build steps for windows. If you'd like to add some, feel free to do a pull request!
