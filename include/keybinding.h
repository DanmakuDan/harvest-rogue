/*
    harvest-rogue is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    harvest-rogue is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with harvest-rogue.  If not, see <http://www.gnu.org/licenses/>.     */

#ifndef HARVEST_ROGUE_KEYBINDING_H
#define HARVEST_ROGUE_KEYBINDING_H

#include <map>

#define IK_LEFT_ARROW  0404
#define IK_UP_ARROW    0403
#define IK_RIGHT_ARROW 0405
#define IK_DOWN_ARROW  0402
#define IK_SPACEBAR    ' '
#ifdef WIN32
	#define IK_RETURN_KEY  13
#else
	#define IK_RETURN_KEY  10
#endif
#define IK_ESCAPE      27

#define ACTION_UNASSIGNED -1
#define ACTION_MOVE_UP 0
#define ACTION_MOVE_DOWN 1
#define ACTION_MOVE_LEFT 2
#define ACTION_MOVE_RIGHT 3
#define ACTION_OPEN_INVENTORY 4
#define ACTION_OPEN_ACTION_LIST 5
#define ACTION_GAME_MENU 6
#define ACTION_USE_TOOL 7

typedef int Action;

class Keybinding {
	public:
		Keybinding();
		~Keybinding();

      Action GetAction(int key);

   private:
      std::map<int, Action> keybindings;
};

#endif // HARVEST_ROGUE_KEYBINDING_H
