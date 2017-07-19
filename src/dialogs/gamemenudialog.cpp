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
#include "player.h"
#include "inventorydialog.h"

GameMenuDialog::GameMenuDialog() {
   this->SelectedOption = GameMenuDialogOption::Status;
}

void GameMenuDialog::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuDown)) {
      if (this->SelectedOption >= GameMenuDialogOption::_MAX - 1) {
         this->SelectedOption = GameMenuDialogOption::GameMenuDialogOption(0);
      } else {
         this->SelectedOption = GameMenuDialogOption::GameMenuDialogOption(this->SelectedOption + 1);
      }
   }

   if (Action::Requested(action, Action::MenuUp)) {
      if (this->SelectedOption <= GameMenuDialogOption::GameMenuDialogOption(0)) {
         this->SelectedOption = GameMenuDialogOption::GameMenuDialogOption(GameMenuDialogOption::_MAX - 1);
      } else {
         this->SelectedOption = GameMenuDialogOption::GameMenuDialogOption(this->SelectedOption - 1);
      }
   }

   if (Action::Requested(action, Action::MenuAccept)) {
      ExecuteSelectedAction();
   }

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }

}

void GameMenuDialog::Render() {
   auto dialogHeight = GameMenuDialogOption::_MAX;
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (GAMEMENU_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (dialogHeight / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, GAMEMENU_DIALOG_WIDTH, dialogHeight + 2, "Game Menu");

   auto btnLeft = dialogLeft + 1;
   auto btnWidth = GAMEMENU_DIALOG_WIDTH - 2;
   auto btnTop = dialogTop;

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Status",
      this->SelectedOption == GameMenuDialogOption::Status);

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Inventory",
      this->SelectedOption == GameMenuDialogOption::Inventory);

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Actions",
      this->SelectedOption == GameMenuDialogOption::Actions);

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Save Game",
      this->SelectedOption == GameMenuDialogOption::SaveGame);

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Quit to Main Menu",
      this->SelectedOption == GameMenuDialogOption::Quit);
}

void GameMenuDialog::ExecuteSelectedAction() const {
   switch (this->SelectedOption) {
   case GameMenuDialogOption::Inventory:
      GameState::Get().PushDialog(InventoryDialog::Construct(Player::Get().AsItemContainer()));
      break;

   case GameMenuDialogOption::Actions:
      GameState::Get().PushDialog(ActionDialog::Construct());
      break;

   case GameMenuDialogOption::Quit:
      GameState::Get().SetCurrentScene(MainMenu::Construct());
      break;
   }
}
