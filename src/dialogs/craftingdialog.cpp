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
#include "screen.h"
#include "craftingdialog.h"
#include "craftingconfirmdialog.h"
#include "craftable.h"
#include "player.h"
#include <sstream>
#include <iomanip>

CraftingDialog::CraftingDialog() {
   this->CurrentCategory = "All";

   auto itemDatabase = GameState::Get().GetItemDatabase();
   for (auto entry : itemDatabase) {
      auto item = entry.second;
      if (!item->HasInterface(ItemInterfaceType::Craftable)) {
         continue;
      }

      auto itemCategories = item->GetItemCategories();
      for (auto itemCategory : itemCategories) {
         this->AddItemToCategory("All", item);
         this->AddItemToCategory(ItemCategory::GetDescription(itemCategory), item);
      }
   }

   this->CurrentItem = this->ItemCategories[this->CurrentCategory].front();
}

void CraftingDialog::SelectPreviousCategory() {
   auto currentPos = this->ItemCategories.find(this->CurrentCategory);
   this->CurrentItem = nullptr;
   if (currentPos == this->ItemCategories.begin()) {
      this->CurrentCategory = (--this->ItemCategories.end())->first;
   }
   else {
      this->CurrentCategory = (--currentPos)->first;
   }
   this->CurrentItem = this->ItemCategories[this->CurrentCategory].front();
}

void CraftingDialog::SelectNextCategory() {
   auto currentPos = this->ItemCategories.find(this->CurrentCategory);
   this->CurrentItem = nullptr;
   if (++currentPos == this->ItemCategories.end()) {
      this->CurrentCategory = this->ItemCategories.begin()->first;
   }
   else {
      this->CurrentCategory = currentPos->first;
   }

   this->CurrentItem = this->ItemCategories[this->CurrentCategory].front();
}

void CraftingDialog::SelectNextItem() {
   auto category = this->ItemCategories[this->CurrentCategory];
   auto index = std::find(category.begin(), category.end(), this->CurrentItem);
   if (index == category.end()-1) {
      this->CurrentItem = category.front();
      return;
   }
   
   this->CurrentItem = *(++index);
}

void CraftingDialog::SelectPreviousItem() {
   auto category = this->ItemCategories[this->CurrentCategory];
   auto index = std::find(category.begin(), category.end(), this->CurrentItem);
   if (index == category.begin()) {
      this->CurrentItem = category.back();
      return;
   }

   this->CurrentItem = *(--index);
}

void CraftingDialog::AddItemToCategory(std::string category, ItemPtr item) {
   if (this->ItemCategories.find(category) == this->ItemCategories.end()) {
      this->ItemCategories[category] = std::vector<ItemPtr>();
   }

   this->ItemCategories[category].push_back(ItemPtr(item));
}

void CraftingDialog::OnKeyPress(int key) {
   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuAccept)) {
      if (this->CurrentItem != nullptr) {
         GameState::Get().PushDialog(CraftingConfirmDialog::Construct(this->CurrentItem));
      }
   }
   else if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }
   else if (Action::Requested(action, Action::MenuLeft)) {
      this->SelectPreviousCategory();
   }
   else if (Action::Requested(action, Action::MenuRight)) {
      this->SelectNextCategory();
   }
   else if (Action::Requested(action, Action::MenuUp)) {
      this->SelectPreviousItem();
   }
   else if (Action::Requested(action, Action::MenuDown)) {
      this->SelectNextItem();
   }
}

void CraftingDialog::Render() {
   this->DrawDialogHeader(3, 3, Screen::Get().GetWidth() - 6, 3);
   this->DrawNavigationDialog();
   if (this->CurrentItem != nullptr) {
      this->DrawDetailsDialog();
   }
}

void CraftingDialog::DrawDialogHeader(int x, int y, int width, int height) {
   std::string dialogTitle = "Crafting";

   Screen::Get().WriteWindow(x, y, width, height);
   auto textLeft = x + (width / 2) - (int(dialogTitle.size()) / 2);
   Screen::Get().WriteText(textLeft, y + 1, dialogTitle);
}

void CraftingDialog::DrawNavigationDialog() {
   auto dialogLeft = 3;
   auto dialogTop = 6;
   auto dialogWidth = (Screen::Get().GetWidth() / 2) - 3;
   auto dialogHeight = (Screen::Get().GetHeight() - 12);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, Screen::Get().GetWidth() - 6, 3);
   Screen::Get().WriteWindow(dialogLeft, dialogTop + 3, dialogWidth, dialogHeight);
   Screen::Get().WriteText(dialogLeft + 1, dialogTop + 1, "Category:", Color::Silver);
   Screen::Get().WriteText(dialogLeft + 11, dialogTop + 1, this->CurrentCategory, Color::White);

   auto items = this->ItemCategories[this->CurrentCategory];
   auto yPos = dialogTop + 3;
   for (auto item : items) {
      if (this->CurrentItem == item) {
         Screen::Get().WriteButton(dialogLeft + 1, ++yPos, dialogWidth - 2, "", true);
         Screen::Get().WriteText(dialogLeft + 2, yPos, item->GetName(), Color::Inverse(Color::White));
         continue;
      }
      Screen::Get().WriteText(dialogLeft + 2, ++yPos, item->GetName());
   }
}

void CraftingDialog::DrawDetailsDialog() {
   auto item = this->CurrentItem->GetInterface<Craftable>(ItemInterfaceType::Craftable);
   auto dialogLeft = (Screen::Get().GetWidth() / 2);
   auto dialogTop = 9;
   auto dialogWidth = (Screen::Get().GetWidth() - (Screen::Get().GetWidth() / 2)) - 3;
   auto dialogHeight = (Screen::Get().GetHeight() - 12);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, dialogWidth, dialogHeight);

   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 1, "Description:", Color::Silver);
   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 2, this->CurrentItem->GetDescription(), Color::BrightYellow);

   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 4, "Crafting Time (each):", Color::Silver);
   {
      std::stringstream ss;
      ss.imbue(std::locale(""));
      ss << std::fixed << item->GetSecondsToCraft();
      ss << " seconds";
      Screen::Get().WriteText(dialogLeft + 2, dialogTop + 5, ss.str(), Color::BrightYellow);
   }

   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 7, "Bill of Materials:", Color::Silver);
   Screen::Get().WriteText(dialogLeft + 2, dialogTop + 8, "Name", Color::White);
   Screen::Get().WriteText(dialogLeft + dialogWidth - 12, dialogTop + 8, "Need", Color::White);
   Screen::Get().WriteText(dialogLeft + dialogWidth - 6, dialogTop + 8, "Have", Color::White);

   auto y = dialogTop + 8;
   for(auto materialRecord : item->GetRequiredMaterials()) {
      y++;
      auto materialName = materialRecord.first;
      auto materialCount = materialRecord.second;
      auto playerMaterialCount = Player::Get().GetAmountOfThisItemHeld(materialName);
      Screen::Get().WriteText(dialogLeft + 2, y, materialName, Color::White);

      {
         std::stringstream ss;
         ss << std::right << std::setw(4) << materialCount;
         Screen::Get().WriteText(dialogLeft + dialogWidth - 12, y, ss.str(), Color::White);
      }
      {
         auto color = playerMaterialCount < materialCount ? Color::BrightRed : Color::BrightGreen;
         std::stringstream ss;
         ss << std::right << std::setw(4) << playerMaterialCount;
         Screen::Get().WriteText(dialogLeft + dialogWidth - 6, y, ss.str(), color);
      }
   }
}
