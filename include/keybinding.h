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

namespace Action {
   typedef unsigned long Action;

   const Action Unassigned           = 0x0000000;
   const Action MoveUp               = 0x0000001;
   const Action MoveDown             = 0x0000002;
   const Action MoveLeft             = 0x0000004;
   const Action MoveRight            = 0x0000008;
   const Action OpenInventory        = 0x0000010;
   const Action OpenActionList       = 0x0000020;
   const Action GameMenu             = 0x0000040;
   const Action UseTool              = 0x0000080;
   const Action MenuUp               = 0x0000100;
   const Action MenuDown             = 0x0000200;
   const Action MenuLeft             = 0x0000400;
   const Action MenuRight            = 0x0000800;
   const Action MenuAccept           = 0x0001000;
   const Action MenuCancel           = 0x0002000;
   const Action PickUpItemFromGround = 0x0004000;

   static bool Requested(Action source, Action test) { return (source & test) > 0; }
};


class Keybinding {
public:
   Keybinding();

   ~Keybinding();

   Action::Action GetAction(int key);

private:
   std::map<int, Action::Action> keybindings;
};

#endif // HARVEST_ROGUE_KEYBINDING_H
