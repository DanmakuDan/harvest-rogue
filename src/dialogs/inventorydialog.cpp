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
#include "toolactiondialog.h"
#include "obtainable.h"

InventoryDialog::InventoryDialog(ItemContainerPtr itemContainer) {
   this->InventoryOffset = 0;
   this->SelectedInventoryItem = 0;
   this->ItemContainer = itemContainer;
}

void InventoryDialog::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   auto totalInventoryCount = (int)this->ItemContainer->GetAllItems().size();

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }

   if (totalInventoryCount <= 0) {
      return;
   }

   if (Action::Requested(action, Action::MenuDown)) {
      this->SelectedInventoryItem++;
      if (this->SelectedInventoryItem >= totalInventoryCount) {
         this->SelectedInventoryItem = 0;
      }
   }

   if (Action::Requested(action, Action::MenuUp)) {
      this->SelectedInventoryItem--;
      if (this->SelectedInventoryItem < 0) {
         this->SelectedInventoryItem = totalInventoryCount - 1;

      }
   }

   // Ensure the inventory scrolls properly
   if (this->SelectedInventoryItem >= (INVENTORY_MAX_ITEMS_SHOWN / 2)) {
      this->InventoryOffset = this->SelectedInventoryItem - (INVENTORY_MAX_ITEMS_SHOWN / 2);
      if (this->InventoryOffset >= (totalInventoryCount - INVENTORY_MAX_ITEMS_SHOWN)) {
         this->InventoryOffset = (totalInventoryCount - INVENTORY_MAX_ITEMS_SHOWN);
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
   auto inventoryCount = (int)this->ItemContainer->GetAllItems().size();

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

   for(auto i = 0; i < dialogHeight - 2; i++) {
      auto inventoryIndex = i + this->InventoryOffset;
      if (inventoryIndex >= this->ItemContainer->GetAllItems().size()) {
         break;
      }

      if (i >= INVENTORY_MAX_ITEMS_SHOWN) {
         break;
      }
      auto inventoryItem = this->ItemContainer->GetAllItems()[inventoryIndex];
      std::stringstream ss;
      ss << inventoryItem->GetName();
      auto obtainableInterface = inventoryItem->GetInterface<Obtainable>(ItemInterfaceType::Obtainable);
      if (obtainableInterface->GetIsStackable()) {
         ss << " [" << inventoryItem->GetCount() << "]";
      }
      Screen::Get().WriteButton(btnLeft, ++btnTop, btnWidth, ss.str(), inventoryIndex == this->SelectedInventoryItem);

      if (inventoryIndex == this->SelectedInventoryItem) {
         Screen::Get().WriteText(btnLeft + 1, dialogTop + dialogHeight - 2, inventoryItem->GetDescription(), Color::Magenta);
      }
   }



}

void InventoryDialog::ExecuteSelectedAction() {
   if (this->SelectedInventoryItem == -1) {
      return;
   }
   auto inventoryItem = ItemPtr(this->ItemContainer->GetAllItems()[this->SelectedInventoryItem]);
   GameState::Get().PushDialog(ToolActionDialog::Construct(inventoryItem));

   // Standard prop item
}
