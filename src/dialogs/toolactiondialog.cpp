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

#include "toolactiondialog.h"
#include "prop.h"
#include "screen.h"
#include "input.h"
#include "gamestate.h"
#include "player.h"

ToolActionDialog::ToolActionDialog(std::shared_ptr<ITool> tool) {
   this->Tool = std::shared_ptr<ITool>(tool);
   this->SelectedOption = ToolActionDialogOption::EquipTool;
}

void ToolActionDialog::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuDown)) {
      if (this->SelectedOption >= ToolActionDialogOption::_MAX - 1) {
         this->SelectedOption = (ToolActionDialogOption::ToolActionDialogOption) 0;
      } else {
         this->SelectedOption = (ToolActionDialogOption::ToolActionDialogOption) ((int) this->SelectedOption + 1);
      }
   }

   if (Action::Requested(action, Action::MenuUp)) {
      if (this->SelectedOption <= (ToolActionDialogOption::ToolActionDialogOption) 0) {
         this->SelectedOption = (ToolActionDialogOption::ToolActionDialogOption)
               ((int) ToolActionDialogOption::_MAX - 1);
      } else {
         this->SelectedOption = (ToolActionDialogOption::ToolActionDialogOption) ((int) this->SelectedOption - 1);
      }
   }

   if (Action::Requested(action, Action::MenuAccept)) {
      ExecuteSelectedAction();
   }

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }

}

void ToolActionDialog::Render() {
   auto dialogHeight = ToolActionDialogOption::_MAX + 2;
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (TOOLACTIN_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (dialogHeight / 2);
   auto prop = dynamic_cast<IProp*>(this->Tool.get());

   Screen::Get().WriteWindow(dialogLeft, dialogTop, TOOLACTIN_DIALOG_WIDTH, dialogHeight, prop->GetName());

   auto btnLeft = dialogLeft + 1;
   auto btnWidth = TOOLACTIN_DIALOG_WIDTH - 2;
   auto btnTop = dialogTop;

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Equip",
                             this->SelectedOption == ToolActionDialogOption::EquipTool);

   Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, "Drop on ground",
                             this->SelectedOption == ToolActionDialogOption::DropTool);
}

void ToolActionDialog::ExecuteSelectedAction() {
   switch(this->SelectedOption) {
      case ToolActionDialogOption::EquipTool:
         if (Player::Get().GetCurrentTool()) {
            Player::Get().UnequipCurrentTool();
         }

         Player::Get().EquipFromInventory(this->Tool);
         GameState::Get().ClearAllDialogs();
         break;
      case ToolActionDialogOption::DropTool:
         Player::Get().DropInventoryItemOnGround(std::dynamic_pointer_cast<IProp>(this->Tool));
         GameState::Get().ClearAllDialogs();
         break;
   }
}
