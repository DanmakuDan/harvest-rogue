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
   this->SelectedOption = MainMenuDialogOption::NewGame;
   Input::Get().DisableInputTimeout();
}

void MainMenu::InitializeScreen() {
   this->Render();
}

void MainMenu::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);
   if (Action::Requested(action, Action::MenuDown)) {
      if (this->SelectedOption >= MainMenuDialogOption::_MAX - 1) {
         this->SelectedOption = MainMenuDialogOption::MainMenuDialogOption(0);
      }
      else {
         this->SelectedOption = MainMenuDialogOption::MainMenuDialogOption(this->SelectedOption + 1);
      }
   }

   if (Action::Requested(action, Action::MenuUp)) {
      if (this->SelectedOption <= MainMenuDialogOption::MainMenuDialogOption(0)) {
         this->SelectedOption = MainMenuDialogOption::MainMenuDialogOption(MainMenuDialogOption::_MAX - 1);
      }
      else {
         this->SelectedOption = MainMenuDialogOption::MainMenuDialogOption(this->SelectedOption - 1);
      }
   }

   if (Action::Requested(action, Action::MenuAccept)) {
      ExecuteSelectedAction();
   }
   
   this->Render();
}

void MainMenu::ExecuteSelectedAction() {
   switch (this->SelectedOption) {
      case MainMenuDialogOption::NewGame:
         GameState::Get().InitializeNewGame();
         GameState::Get().SetCurrentScene(Game::Construct());
         break;
      case MainMenuDialogOption::Quit: 
         GameState::Get().Terminate();
         return;
   }
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

   Screen::Get().WriteCenterText(top, logo, Color::White);
   this->DrawMenu();

   Screen::Get().EndScreenUpdate();
}

void MainMenu::DrawMenu() {
   /*
   auto top = (Screen::Get().GetHeight() / 2) + 4;
   auto buttonLeft = (Screen::Get().GetWidth() / 2) - 15;
   Screen::Get().WriteButton(buttonLeft, top++, 30, "New Game", 0 == this->SelectedButton);
   Screen::Get().WriteButton(buttonLeft, top++, 30, "Continue", 1 == this->SelectedButton);
   Screen::Get().WriteButton(buttonLeft, top++, 30, "Quit", 2 == this->SelectedButton);
   */
   auto dialogHeight = MainMenuDialogOption::_MAX;
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (MAINMENU_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() - (dialogHeight + 3));
   Screen::Get().WriteWindow(dialogLeft, dialogTop, MAINMENU_DIALOG_WIDTH, dialogHeight + 2, "Main Menu");

   auto btnLeft = dialogLeft + 1;
   auto btnWidth = MAINMENU_DIALOG_WIDTH - 2;
   auto btnTop = dialogTop;

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "New Game", this->SelectedOption == MainMenuDialogOption::NewGame);
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Continue", this->SelectedOption == MainMenuDialogOption::Continue);
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Quit", this->SelectedOption == MainMenuDialogOption::Quit);
}

