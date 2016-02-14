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

#include <memory>
#include <sstream>
#include "inventorydialog.h"
#include "screen.h"
#include "input.h"
#include "gamestate.h"
#include "player.h"
#include "toolactiondialog.h"
#include "obtainable.h"

InventoryDialog::InventoryDialog() {
   this->InventoryOffset = 0;
   this->SelectedInventoryItem = 0;
}

void InventoryDialog::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   auto playerTotalInventoryCount = (int)Player::Get().GetInventory().size();

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }

   if (playerTotalInventoryCount <= 0) {
      return;
   }

   if (Action::Requested(action, Action::MenuDown)) {
      this->SelectedInventoryItem++;
      if (this->SelectedInventoryItem >= playerTotalInventoryCount) {
         this->SelectedInventoryItem = 0;
      }
   }

   if (Action::Requested(action, Action::MenuUp)) {
      this->SelectedInventoryItem--;
      if (this->SelectedInventoryItem < 0) {
         this->SelectedInventoryItem = playerTotalInventoryCount - 1;

      }
   }

   // Ensure the inventory scrolls properly
   if (this->SelectedInventoryItem >= (INVENTORY_MAX_ITEMS_SHOWN / 2)) {
      this->InventoryOffset = this->SelectedInventoryItem - (INVENTORY_MAX_ITEMS_SHOWN / 2);
      if (this->InventoryOffset >= (playerTotalInventoryCount - INVENTORY_MAX_ITEMS_SHOWN)) {
         this->InventoryOffset = (playerTotalInventoryCount - INVENTORY_MAX_ITEMS_SHOWN);
      }
      if (this->InventoryOffset < 0) {
         this->InventoryOffset = 0;
      }
   } else {
      this->InventoryOffset = 0;
   }

   if (Action::Requested(action, Action::MenuAccept)) {
      ExecuteSelectedAction();
   }



}

void InventoryDialog::Render() {
   auto inventoryCount = (int)Player::Get().GetInventory().size();

   if (inventoryCount == 0) {
      this->SelectedInventoryItem = -1;
      auto dialogLeft = (Screen::Get().GetWidth() / 2) - (INVENTORY_DIALOG_WIDTH / 2);
      auto dialogTop = (Screen::Get().GetHeight() / 2) - 2;
      Screen::Get().WriteWindow(dialogLeft, dialogTop, INVENTORY_DIALOG_WIDTH, 3, "Inventory");
      Screen::Get().WriteText(dialogLeft + 2, dialogTop + 1, "(nothing)", Color::Gray);
      return;
   }

   auto dialogHeight = inventoryCount + 4;
   if (dialogHeight > INVENTORY_MAX_ITEMS_SHOWN + 4) {
      dialogHeight = INVENTORY_MAX_ITEMS_SHOWN + 4;
   }

   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (INVENTORY_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (dialogHeight / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, INVENTORY_DIALOG_WIDTH, dialogHeight, "");
   Screen::Get().WriteWindow(dialogLeft, dialogTop, INVENTORY_DIALOG_WIDTH, dialogHeight - 2, "Inventory");
   auto btnLeft = dialogLeft + 1;
   auto btnWidth = INVENTORY_DIALOG_WIDTH - 2;
   auto btnTop = dialogTop;

   auto playerInventory = Player::Get().GetInventory();

   for(auto i = 0; i < dialogHeight - 2; i++) {
      auto inventoryIndex = i + this->InventoryOffset;
      if (inventoryIndex >= playerInventory.size()) {
         break;
      }

      if (i >= INVENTORY_MAX_ITEMS_SHOWN) {
         break;
      }
      auto inventoryItem = playerInventory[inventoryIndex];
      std::stringstream ss;
      ss << inventoryItem->ItemTarget->GetName();
      auto obtainableInterface = inventoryItem->ItemTarget->GetInterface<Obtainable>(ItemInterfaceType::Obtainable);
      if (obtainableInterface->GetIsStackable()) {
         ss << " [" << inventoryItem->StackSize << "]";
      }
      Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, ss.str(), inventoryIndex == this->SelectedInventoryItem);

      if (inventoryIndex == this->SelectedInventoryItem) {
         Screen::Get().WriteText(btnLeft + 1, dialogTop + dialogHeight - 2, inventoryItem->ItemTarget->GetDescription(), Color::Magenta);
      }
   }



}

void InventoryDialog::ExecuteSelectedAction() {
   if (this->SelectedInventoryItem == -1) {
      return;
   }
   auto inventoryItem = std::shared_ptr<Item>(Player::Get().GetInventory()[this->SelectedInventoryItem]->ItemTarget);
   auto toolItem = std::dynamic_pointer_cast<Item>(inventoryItem);
   if (toolItem != nullptr) {
      // Tool item
      GameState::Get().PushDialog(ToolActionDialog::Construct(toolItem));
      return;
   }

   // Standard prop item
}
