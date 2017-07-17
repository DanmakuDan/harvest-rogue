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

#include "input.h"
#include "gamestate.h"
#include "mainmenu.h"


int main() {
   GameState::Get().SetCurrentScene(MainMenu::Construct());

   while (GameState::Get().IsActive()) {

      if (GameState::Get().TransitionScene()) {
         GameState::Get().GetCurrentScene()->InitializeScreen();
      }

      auto ch = Input::Get().WaitForAndGetKeyPress();
      switch (ch) {
         case 0:
            continue;
         case RESIZE_KEY:
            GameState::Get().GetCurrentScene()->InitializeScreen();
            continue;
         case KILL_KEY:
            GameState::Get().Terminate();
            continue;
         default:
            GameState::Get().GetCurrentScene()->OnKeyPress(ch);
      }
   }

   return 0;
}

#ifdef WIN32
#include <Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	main();
}
#endif
