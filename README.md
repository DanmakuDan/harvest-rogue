# Harvest-Rogue

**We are actively looking for contributers of all types!**
The graphical front-end will begin development next week, but until then, we need help with the website, game code, testing, and porting efforts. Open an issue, or click on the chat badge below to start contributing! If you don't feel like contributing, but are interested in seeing this progress, please star this repository to help increase its visibility. It really helps!

[![Gitter](https://badges.gitter.im/essial/harvest-rogue.svg)](https://gitter.im/essial/harvest-rogue?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Build Status](https://travis-ci.org/essial/harvest-rogue.svg?branch=master)](https://travis-ci.org/essial/harvest-rogue)

[HarvestRogue subreddit](https://www.reddit.com/r/harvestrogue/)

![Title](http://i.imgur.com/IfmnANS.png) 
![Gameplay](http://i.imgur.com/OPFYIPP.png)

Welcome to the project page for Harvest-Rogue, a Harvest Moon-inspired roguelike! This project is very early in development, so check back often for updates!

While testing the game, pressing F12 at any time will immediately quit the game. This will remain in place during development.

## Basic Controlls
* Press <kbd>Enter</kbd> to open the main menu
* Press arrow keys to navigate dialogs and move on the map
* Press <kbd>F12</kbd> to immediately terminate the game at any time (temporary)
* Press <kbd>ESC</kbd> to close dialogs without performing an action
* Press <kbd>a</kbd> to open the actions menu directly
* Press <kbd>i</kbd> to open the inventory menu directly
* Press <kbd>SPACEBAR</kbd> to use the currently equipped tool

## Tasks
The following tasks are currently being worked on:

* General Features
    - Statistics Window Area
    - Inventory Management

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

### Windows (Visual Studio)
* Install Visual Studio or [Visual Studio Express](https://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx) if you haven't already.
* Install the [CMake for Windows](https://cmake.org/download/) msi package if you haven't already.
* Create a folder in your <code>harvest-rogue</code> root folder called <code>output</code>
* Run cmake-gui
* For the "Where is the source code" field, select the base <code>harvest-rogue</code> folder.
* For the "Where to build the binaries" field, select the base <code>harvest-rogue/output</code> folder you just created.
* Click the <code>Configure</code> button. Wait until it finishes.
* Click the <code>Generate</code> button. Wait until it finishes.
* Close cmake-gui and navigate to your <code>harvest-rogue/output</code> folder, and open the <code>harvest_rogue.sln</code> file.
* Right-click the <code>harvest_rogue</code> project and click <code>Set as StartUp Project</code>
* Rebuild and run!

Notes:

* If you change the CMake file, you can re-build the <code>ZERO_CHECK</code> project, and it will re-generate the project files with the new changes.
* Do **not** add new files directly into the solution. If you want to add new files, add them directly to the folder in windows explorer, and re-run <code>ZERO_CHECK</code>. Visual studio will misplace the source files if you do not do this!
