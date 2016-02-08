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
   this->keybindings['a'] = Action::OpenActionList;
   this->keybindings['A'] = Action::OpenActionList;
   this->keybindings['i'] = Action::OpenInventory;
   this->keybindings['I'] = Action::OpenInventory;

   this->keybindings[IK_UP_ARROW] = Action::MoveUp | Action::MenuUp;
   this->keybindings[IK_DOWN_ARROW] = Action::MoveDown | Action::MenuDown ;
   this->keybindings[IK_LEFT_ARROW] = Action::MoveLeft | Action::MenuLeft ;
   this->keybindings[IK_RIGHT_ARROW] = Action::MoveRight | Action::MenuRight ;
   this->keybindings[IK_RETURN_KEY] = Action::GameMenu | Action::MenuAccept ;
   this->keybindings[IK_SPACEBAR] = Action::UseTool;
   this->keybindings[IK_ESCAPE] = Action::MenuCancel;

   // Vi-style movement this->keys
   this->keybindings['j'] = Action::MoveDown;
   this->keybindings['k'] = Action::MoveUp;
   this->keybindings['l'] = Action::MoveRight;
   this->keybindings['h'] = Action::MoveLeft;
}

Keybinding::~Keybinding() {
}

Action::Action Keybinding::GetAction(int key) {
   auto it = this->keybindings.find(key);

   if (it == this->keybindings.end()) {
      return Action::Unassigned ;
   } else {
      return it->second;
   }
}
