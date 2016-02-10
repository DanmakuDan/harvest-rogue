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

#include <gamestate.h>
#include "mainmenu.h"
#include "input.h"
#include "game.h"
#include "screen.h"

MainMenu::MainMenu() {
   this->SelectedButton = 0;
   Input::Get().DisableInputTimeout();
}

void MainMenu::InitializeScreen() {
   this->Render();
}

void MainMenu::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuDown)) {
      if (2 < ++this->SelectedButton) {
         this->SelectedButton = 0;
      }
   }

   if (Action::Requested(action, Action::MenuUp)) {
      if (0 > --this->SelectedButton) {
         this->SelectedButton = 2;
      }
   }

   if (Action::Requested(action, Action::MenuAccept)) {
      switch (this->SelectedButton) {
         case 0: // New Game
            GameState::Get().InitializeNewGame();
            GameState::Get().SetCurrentScene(Game::Construct());
            break;
         case 2: // Quit
            GameState::Get().Terminate();
            return;
      }
   }

   this->Render();
}


void MainMenu::Render() {
   Screen::Get().BeginScreenUpdate();

   auto top = (Screen::Get().GetHeight() / 2) - 10;
   std::string logo(
         "                                                    .:.        \n"
               "                                                .:. \\|/  .:.\n"
               "                   _                            \\|/  |   \\|/\n"
               "                 _/_\\_   ___                 <#> |  \\|<#> |\n"
               "                  (\")   /.-.\\                   \\|<#>|/  \\| /\n"
               "        _        //U\\\\  |(\")|                    |  \\| /<#>/\n"
               "       ( )   _   \\|_|/  /)v(\\                  \\ |/  |/  \\|\n"
               "      (_` )_('>   | |   \\/~\\/                   \\|   |    |/\n"
               "      (__,~_)8    |||   //_\\\\                    |/ \\| / \\| /\n"
               "         _YY_    _[|]_ /_____\\                  \\|   |/   |/\n"
               "  \"\"\"\"\"\"\"\"'\"\"'\"\"'\"\"\"\"\"'\"\"\"\"'\"\"'\"\"\"'\"\"\"\"\"''\"'\"\"\"^^^^^^^^^^^^^^^^\n"
               "                         Harvest-Rogue                         "
   );

   Screen::Get().WriteCenterText(top, logo, CLR_WHITE);
   this->DrawMenu();

   Screen::Get().EndScreenUpdate();
}

void MainMenu::DrawMenu() {
   auto top = (Screen::Get().GetHeight() / 2) + 4;
   auto buttonLeft = (Screen::Get().GetWidth() / 2) - 15;
   Screen::Get().WriteButton(buttonLeft, top++, 30, "New Game", 0 == this->SelectedButton);
   Screen::Get().WriteButton(buttonLeft, top++, 30, "Continue", 1 == this->SelectedButton);
   Screen::Get().WriteButton(buttonLeft, top++, 30, "Quit", 2 == this->SelectedButton);
}

