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

ActionDialog::ActionDialog() {
   this->SelectedOption = eActionDialogOptions::OptPickUp;
}

void ActionDialog::OnKeyPress(int key) {
   switch (key) {
      case IK_DOWN_ARROW:
         if (this->SelectedOption >= eActionDialogOptions::eActionDialogOptionsMax - 1) {
            this->SelectedOption = (eActionDialogOptions) 0;
         } else {
            this->SelectedOption = (eActionDialogOptions) ((int) this->SelectedOption + 1);
         }
         break;
      case IK_UP_ARROW:
         if (this->SelectedOption <= (eActionDialogOptions) 0) {
            this->SelectedOption = (eActionDialogOptions) ((int) eActionDialogOptions::eActionDialogOptionsMax - 1);
         } else {
            this->SelectedOption = (eActionDialogOptions) ((int) this->SelectedOption - 1);
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

void ActionDialog::Render() {
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (ACTION_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (ACTION_DIALOG_HEIGHT / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, ACTION_DIALOG_WIDTH, ACTION_DIALOG_HEIGHT, "Actions");

   auto btnLeft = dialogLeft + 1;
   auto btnWidth = ACTION_DIALOG_WIDTH - 2;
   auto btnTop = dialogTop;
   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Pick up",
                             this->SelectedOption == eActionDialogOptions::OptPickUp);

}

void ActionDialog::ExecuteSelectedAction() {
   switch(this->SelectedOption) {
      case eActionDialogOptions::OptPickUp:
         Player::Get().PickUpItemFromGround();
         GameState::Get().ClearAllDialogs();
         break;
   }
}
