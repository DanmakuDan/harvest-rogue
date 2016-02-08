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
#include "gamemenudialog.h"
#include "screen.h"
#include "gamestate.h"
#include "mainmenu.h"
#include "actiondialog.h"
#include "inventorydialog.h"

GameMenuDialog::GameMenuDialog() {
   this->SelectedOption = eGameMenuDialogOptions::OptStatus;
}

void GameMenuDialog::OnKeyPress(int key) {
   switch (key) {
      case IK_DOWN_ARROW:
         if (this->SelectedOption >= eGameMenuDialogOptions::eGameMenuDialogOptionsMax - 1) {
            this->SelectedOption = (eGameMenuDialogOptions) 0;
         } else {
            this->SelectedOption = (eGameMenuDialogOptions) ((int) this->SelectedOption + 1);
         }
         break;
      case IK_UP_ARROW:
         if (this->SelectedOption <= (eGameMenuDialogOptions) 0) {
            this->SelectedOption = (eGameMenuDialogOptions) ((int) eGameMenuDialogOptions::eGameMenuDialogOptionsMax - 1);
         } else {
            this->SelectedOption = (eGameMenuDialogOptions) ((int) this->SelectedOption - 1);
         }
         break;
      case IK_RETURN_KEY:
         ExecuteSelectedAction();
         break;
      case IK_ESCAPE:
         GameState::Get().PopDialog();
         break;
   }
}

void GameMenuDialog::Render() {
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (GAMEMENU_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (GAMEMENU_DIALOG_HEIGHT / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, GAMEMENU_DIALOG_WIDTH, GAMEMENU_DIALOG_HEIGHT, "Game Menu");

   auto btnLeft = dialogLeft + 1;
   auto btnWidth = GAMEMENU_DIALOG_WIDTH - 2;
   auto btnTop = dialogTop;
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Status",
                             this->SelectedOption == eGameMenuDialogOptions::OptStatus);
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Inventory",
                             this->SelectedOption == eGameMenuDialogOptions::OptInventory);
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Actions",
                             this->SelectedOption == eGameMenuDialogOptions::OptActions);
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Save Game",
                             this->SelectedOption == eGameMenuDialogOptions::OptSaveGame);
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Quit to Main Menu",
                             this->SelectedOption == eGameMenuDialogOptions::OptQuit);
}

void GameMenuDialog::ExecuteSelectedAction() {
   switch (this->SelectedOption) {
      case eGameMenuDialogOptions::OptActions:
         GameState::Get().PushDialog(ActionDialog::Construct());
         break;
      case eGameMenuDialogOptions::OptInventory:
         GameState::Get().PushDialog(InventoryDialog::Construct());
         break;
      case eGameMenuDialogOptions::OptQuit:
         GameState::Get().SetCurrentScene(MainMenu::Construct());
         break;
   }
}
