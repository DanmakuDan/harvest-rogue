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

#ifndef HARVEST_ROGUE_INPUT_H
#define HARVEST_ROGUE_INPUT_H

#include <memory>
#include "keybinding.h"

#ifdef WIN32
#define RESIZE_KEY 0x222
#else
#define RESIZE_KEY 0x200
#endif
#define KILL_KEY (0x108+12) // F12

class Input {
private:
   Input();

   Input(Input const &) { };

   Input &operator=(Input const &) { };
public:
   ~Input();

   static Input &Get() {
      static Input input;
      return input;
   }

   int WaitForAndGetKeyPress();

   int GetInputTimeout();

   void SetInputTimeout(int timeout);

   void DisableInputTimeout();

   Action::Action GetActionForKeyPress(int key);

private:
   int InputTimeout;
   Keybinding keybinding;
};


#endif //HARVEST_ROGUE_INPUT_H
