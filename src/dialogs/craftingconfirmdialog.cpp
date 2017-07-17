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

#include "craftingconfirmdialog.h"
#include "input.h"
#include "gamestate.h"
#include "keybinding.h"
#include "screen.h"
#include "craftable.h"
#include "player.h"
#include <sstream>
#include <iomanip>

CraftingConfirmDialog::CraftingConfirmDialog(ItemPtr itemToConstruct) {
   this->ItemToConstruct = itemToConstruct;
   this->AmountToCraft = 1;
   this->HasMaterialToCraft = false;
   this->SelectedOption = CraftingConfirmDialogOption::Craft;
   DialogTitle.append(ItemToConstruct->GetName());
   DialogTitle.append("'s Bill of Materials");
}

void CraftingConfirmDialog::OnKeyPress(int key) {
   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }
   else if (Action::Requested(action, Action::MenuRight)) {
      if (this->SelectedOption >= CraftingConfirmDialogOption::_MAX - 1) {
         this->SelectedOption = CraftingConfirmDialogOption::CraftingConfirmDialogOption(0);
      }
      else {
         this->SelectedOption = CraftingConfirmDialogOption::CraftingConfirmDialogOption(this->SelectedOption + 1);
      }
   }
   else if (Action::Requested(action, Action::MenuLeft)) {
      if (this->SelectedOption <= CraftingConfirmDialogOption::CraftingConfirmDialogOption(0)) {
         this->SelectedOption = CraftingConfirmDialogOption::CraftingConfirmDialogOption(CraftingConfirmDialogOption::_MAX - 1);
      }
      else {
         this->SelectedOption = CraftingConfirmDialogOption::CraftingConfirmDialogOption(this->SelectedOption - 1);
      }
   }
   else if (Action::Requested(action, Action::MenuAccept)) {
      switch (this->SelectedOption) {
      case CraftingConfirmDialogOption::AddOne:
         this->AmountToCraft += 1;
         break;
      case CraftingConfirmDialogOption::AddTen:
         this->AmountToCraft += 10;
         break;
      case CraftingConfirmDialogOption::AddOneHundred:
         this->AmountToCraft += 100;
         break;
      case CraftingConfirmDialogOption::RemoveOne:
         this->AmountToCraft -= 1;
         break;
      case CraftingConfirmDialogOption::RemoveTen:
         this->AmountToCraft -= 10;
         break;
      case CraftingConfirmDialogOption::RemoveOneHundred:
         this->AmountToCraft -= 100;
         break;
      case CraftingConfirmDialogOption::Craft:
         this->Craft();
         break;
      }
      if (this->AmountToCraft < 1) {
this->AmountToCraft = 1;
      }
      else if (this->AmountToCraft > CRAFTINGCONFIRM_DIALOG_MAX_CRAFT) {
         this->AmountToCraft = CRAFTINGCONFIRM_DIALOG_MAX_CRAFT;
      }
   }
}

void CraftingConfirmDialog::Render() {
   auto item = this->ItemToConstruct->GetInterface<Craftable>(ItemInterfaceType::Craftable);
   auto dialogHeight = static_cast<int>(item->GetRequiredMaterials().size()) + 9;
   auto dialogLeft = static_cast<int>(Screen::Get().GetWidth() / 2) - static_cast<int>(CRAFTINGCONFIRM_DIALOG_WIDTH / 2);
   auto dialogTop = static_cast<int>(Screen::Get().GetHeight() / 2) - static_cast<int>(dialogHeight / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, CRAFTINGCONFIRM_DIALOG_WIDTH, dialogHeight, this->DialogTitle);

   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 2, "Crafting Time:", Color::White);
   {
      std::stringstream ss;
      ss.imbue(std::locale(""));
      ss << std::fixed << item->GetSecondsToCraft() * this->AmountToCraft;
      ss << " seconds";
      Screen::Get().WriteText(dialogLeft + 19, dialogTop + 2, ss.str(), Color::BrightYellow);
   }

   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 3, "Amount to Craft:", Color::White);
   {
      std::stringstream ss;
      ss << this->AmountToCraft;
      Screen::Get().WriteText(dialogLeft + 19, dialogTop + 3, ss.str(), Color::BrightYellow);
   }

   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 5, "Name", Color::White);
   Screen::Get().WriteText(dialogLeft + CRAFTINGCONFIRM_DIALOG_WIDTH - 12, dialogTop + 5, "Need", Color::White);
   Screen::Get().WriteText(dialogLeft + CRAFTINGCONFIRM_DIALOG_WIDTH - 6, dialogTop + 5, "Have", Color::White);

   auto y = dialogTop + 5;
   this->HasMaterialToCraft = true;
   for (auto materialRecord : item->GetRequiredMaterials()) {
      y++;
      auto materialName = materialRecord.first;
      auto materialCount = materialRecord.second * this->AmountToCraft;
      auto playerMaterialCount = Player::Get().GetAmountOfThisItemHeld(materialName);

      if (this->HasMaterialToCraft && (playerMaterialCount < materialCount)) {
         this->HasMaterialToCraft = false;
      }

      Screen::Get().WriteText(dialogLeft + 2, y, materialName, Color::White);

      {
         std::stringstream ss;
         ss << std::right << std::setw(4) << materialCount;
         Screen::Get().WriteText(dialogLeft + CRAFTINGCONFIRM_DIALOG_WIDTH - 12, y, ss.str(), Color::White);
      }
      {
         auto color = playerMaterialCount < materialCount ? Color::BrightRed : Color::BrightGreen;
         std::stringstream ss;
         ss << std::right << std::setw(4) << playerMaterialCount;
         Screen::Get().WriteText(dialogLeft + CRAFTINGCONFIRM_DIALOG_WIDTH - 6, y, ss.str(), color);
      }

   }

   Screen::Get().WriteButton(dialogLeft + 3, y + 2, 6, "-100", this->SelectedOption == CraftingConfirmDialogOption::RemoveOneHundred);
   Screen::Get().WriteButton(dialogLeft + 9, y + 2, 6, "-10", this->SelectedOption == CraftingConfirmDialogOption::RemoveTen);
   Screen::Get().WriteButton(dialogLeft + 15, y + 2, 6, "-1", this->SelectedOption == CraftingConfirmDialogOption::RemoveOne);
   Screen::Get().WriteButton(dialogLeft + 21, y + 2, 7, "Craft", this->SelectedOption == CraftingConfirmDialogOption::Craft);
   Screen::Get().WriteButton(dialogLeft + 28, y + 2, 6, "+1", this->SelectedOption == CraftingConfirmDialogOption::AddOne);
   Screen::Get().WriteButton(dialogLeft + 34, y + 2, 6, "+10", this->SelectedOption == CraftingConfirmDialogOption::AddTen);
   Screen::Get().WriteButton(dialogLeft + 40, y + 2, 6, "+100", this->SelectedOption == CraftingConfirmDialogOption::AddOneHundred);
}

void CraftingConfirmDialog::Craft() const {
   if (!this->HasMaterialToCraft) {
      if (this->AmountToCraft > 1) {
         GameState::Get().AddLogMessageFmt("You lack the materials to craft %i copies of %s...", this->AmountToCraft, this->ItemToConstruct->GetName().c_str());
      }
      else {
         GameState::Get().AddLogMessageFmt("You lack the materials to craft the %s...", this->ItemToConstruct->GetName().c_str());
      }
      GameState::Get().ClearAllDialogs();
      return;
   }

   auto item = this->ItemToConstruct->GetInterface<Craftable>(ItemInterfaceType::Craftable);

   for (auto materialRecord : item->GetRequiredMaterials()) {
      auto amountToRemove = materialRecord.second * this->AmountToCraft;

      while (amountToRemove > 0) {
         for (auto playerItem : Player::Get().GetAllItems()) {

            if (playerItem->GetName() != materialRecord.first) {
               continue;
            }

            auto playerItemCount = playerItem->GetCount();
            Player::Get().RemoveItem(playerItem, (playerItemCount < amountToRemove) ? MOVE_AMOUNT_EVERYTHING : amountToRemove);

            amountToRemove = (playerItemCount < amountToRemove)
               ? (amountToRemove - playerItemCount)
               : 0;
            break;
         }
      }
   }

   auto secondsRemaining = item->GetSecondsToCraft() * this->AmountToCraft;
   while (secondsRemaining > 0) {
      GameState::Get().StepSimulation();
      secondsRemaining -= GAMESTATE_SIMULATION_STEP_SECONDS;
   }
   
   for (auto i = 0; i < this->AmountToCraft; i++) {
      auto itemToGive = Item::Clone(this->ItemToConstruct);
      Player::Get().AddItem(itemToGive, 1, false);
   }

   GameState::Get().AddLogMessageFmt("You crafted %i copies of %s...", this->AmountToCraft, this->ItemToConstruct->GetName().c_str());
   GameState::Get().ClearAllDialogs();
}
