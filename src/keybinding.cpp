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

#include "keybinding.h"

Keybinding::Keybinding() {
   // Default keybindings
   this->keybindings['a'] = ACTION_OPEN_ACTION_LIST;
   this->keybindings['A'] = ACTION_OPEN_ACTION_LIST;
   this->keybindings['i'] = ACTION_OPEN_INVENTORY;
   this->keybindings['I'] = ACTION_OPEN_INVENTORY;

   this->keybindings[IK_UP_ARROW] = ACTION_MOVE_UP;
   this->keybindings[IK_DOWN_ARROW] = ACTION_MOVE_DOWN;
   this->keybindings[IK_LEFT_ARROW] = ACTION_MOVE_LEFT;
   this->keybindings[IK_RIGHT_ARROW] = ACTION_MOVE_RIGHT;
   this->keybindings[IK_RETURN_KEY] = ACTION_GAME_MENU;
   this->keybindings[IK_SPACEBAR] = ACTION_USE_TOOL;

   // Vi-style movement this->keys
   this->keybindings['j'] = ACTION_MOVE_DOWN;
   this->keybindings['k'] = ACTION_MOVE_UP;
   this->keybindings['l'] = ACTION_MOVE_RIGHT;
   this->keybindings['h'] = ACTION_MOVE_LEFT;
}

Keybinding::~Keybinding() {
}

Action Keybinding::GetAction(int key) {
   auto it = this->keybindings.find(key);

   if (it == this->keybindings.end()) {
      return ACTION_UNASSIGNED;
   } else {
      return it->second;
   }
}
