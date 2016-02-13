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

#include "gamestate.h"
#include "input.h"
#include "player.h"
#include "actiondialog.h"
#include "screen.h"

ActionDialog::ActionDialog() {
   this->SelectedOption = ActionDialogOption::PickUp;
}

void ActionDialog::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuDown)) {
      if (this->SelectedOption >= ActionDialogOption::_MAX - 1) {
         this->SelectedOption = (ActionDialogOption::ActionDialogOption) 0;
      } else {
         this->SelectedOption = (ActionDialogOption::ActionDialogOption) ((int) this->SelectedOption + 1);
      }
   }

   if (Action::Requested(action, Action::MenuUp)) {
      if (this->SelectedOption <= (ActionDialogOption::ActionDialogOption) 0) {
         this->SelectedOption = (ActionDialogOption::ActionDialogOption) ((int) ActionDialogOption::_MAX - 1);
      } else {
         this->SelectedOption = (ActionDialogOption::ActionDialogOption) ((int) this->SelectedOption - 1);
      }
   }

   if (Action::Requested(action, Action::MenuAccept)) {
      ExecuteSelectedAction();
   }

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }
}

void ActionDialog::Render() {
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (ACTION_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (ACTION_DIALOG_HEIGHT / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, ACTION_DIALOG_WIDTH, ACTION_DIALOG_HEIGHT, "Actions");

   auto btnLeft = dialogLeft + 1;
   auto btnWidth = ACTION_DIALOG_WIDTH - 2;
   auto btnTop = dialogTop;

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Pick up",
                             this->SelectedOption == ActionDialogOption::PickUp);

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Unequip Tool",
                             this->SelectedOption == ActionDialogOption::Unequip);

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Interact With...",
                             this->SelectedOption == ActionDialogOption::InteractWith);

}

void ActionDialog::ExecuteSelectedAction() {
   switch (this->SelectedOption) {

      case ActionDialogOption::PickUp:
         Player::Get().PickUpItemFromGround();
         GameState::Get().ClearAllDialogs();
         break;

      case ActionDialogOption::Unequip:
         Player::Get().UnequipCurrentEquippedItem();
         GameState::Get().ClearAllDialogs();
         break;

      case ActionDialogOption::InteractWith:
         Player::Get().InteractWith();
         break;
   }
}
