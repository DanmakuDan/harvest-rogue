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
#include "config.h"
#include <SDL2/SDL.h>

Input::Input() {
   this->keybinding = Config::provider->GetConfig().GetKeybinding();

   this->InputTimeout = -1;
}

Input::~Input()
{
}

int Input::WaitForAndGetKeyPress()
{
   SDL_Event evt;
   while (true) {
      SDL_WaitEvent(&evt);
      switch (evt.type) {
      case SDL_QUIT:
         GameState::Get().Terminate();
         return 0;
      case SDL_KEYDOWN:
         switch (evt.key.keysym.sym) {
         case SDLK_DOWN: return IK_DOWN_ARROW;
         case SDLK_UP: return IK_UP_ARROW;
         case SDLK_LEFT: return IK_LEFT_ARROW;
         case SDLK_RIGHT: return IK_RIGHT_ARROW;
         case SDLK_SPACE: return IK_SPACEBAR;
         case SDLK_RETURN:
         case SDLK_RETURN2:
            return IK_RETURN_KEY;
         case SDLK_ESCAPE: return IK_ESCAPE;
         default:
            return evt.key.keysym.sym;
         }

      }
   }
}

int Input::GetInputTimeout()
{
   return this->InputTimeout;
}

void Input::SetInputTimeout(int timeout)
{
   this->InputTimeout = timeout;
}

void Input::DisableInputTimeout()
{
   this->SetInputTimeout(-1);
}

Action::Action Input::GetActionForKeyPress(int key)
{
   return this->keybinding.GetAction(key);
}
