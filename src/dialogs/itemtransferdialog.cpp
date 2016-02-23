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

#include "itemtransferdialog.h"
#include "input.h"
#include "gamestate.h"
#include "screen.h"
#include "obtainable.h"

ItemTransferDialog::ItemTransferDialog(ItemContainerPtr firstContainer, ItemContainerPtr secondContainer)
{
   this->FirstContainer = firstContainer;
   this->SecondContainer = secondContainer;
   this->ItemSelectorSide = eItemDialogSide::LeftSide;
   this->FirstItemSelectorIndex = 0;
   this->FirstItemViewOffset = 0;
   this->SecondItemSelectorIndex = 0;
   this->SecondItemViewOffset = 0;
}

void ItemTransferDialog::OnKeyPress(int key)
{
   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuCancel)) {
      if (nullptr != this->SelectedItem) {
         this->SelectedItem = nullptr;
         return;
      }
      GameState::Get().PopDialog();
   }

   if (Action::Requested(action, Action::MenuDown)) {
      this->MoveSelectorInWindow(1);
   }

   if (Action::Requested(action, Action::MenuUp)) {
      this->MoveSelectorInWindow(-1);
   }

   if (Action::Requested(action, Action::MenuLeft) || Action::Requested(action, Action::MenuRight)) {
      this->SwitchSelectedWindow();
   }

   if (Action::Requested(action, Action::MenuAccept)) {
      this->HandleActionButtonPressed();
   }
}

void ItemTransferDialog::Render()
{
   auto screenCenter = Screen::Get().GetWidth() / 2;

   this->DrawDialogHeader(3, 3, Screen::Get().GetWidth() - 6, 3);
   this->DrawItemDialog(3, 10, screenCenter - 3, Screen::Get().GetHeight() - 15, eItemDialogSide::LeftSide);
   this->DrawItemDialog(screenCenter, 10, screenCenter - 3, Screen::Get().GetHeight() - 15, eItemDialogSide::RightSide);
   this->DrawSelectedItemDescription();
}

void ItemTransferDialog::DrawDialogHeader(int x, int y, int width, int height)
{
   std::string dialogTitle = "Inventory Transfer";

   Screen::Get().WriteWindow(x, y, width, height);
   auto textLeft = x + (width / 2) - ((int)dialogTitle.size() / 2);
   Screen::Get().WriteText(textLeft, y + 1, dialogTitle);
}

void ItemTransferDialog::DrawItemDialog(int x, int y, int width, int height, eItemDialogSide side)
{
   auto leftSide = (side == eItemDialogSide::LeftSide);

   auto container         = leftSide ? this->FirstContainer         : this->SecondContainer;
   auto itemSelectorIndex = leftSide ? this->FirstItemSelectorIndex : this->SecondItemSelectorIndex;
   auto viewOffset        = leftSide ? &this->FirstItemViewOffset   : &this->SecondItemViewOffset;

   // Draw the caption bar above the item window
   Screen::Get().WriteWindow(x, y - 3, width, 3);
   auto containerNameLength = (int)container->GetName().size();
   Screen::Get().WriteText(x + (width / 2) - (containerNameLength / 2), 8, container->GetName());

   // Draw the item window frame
   Screen::Get().WriteWindow(x, y, width, height);

   // Draw the items in the dialog
   auto maxItemsToShow = height - 2;
   auto containerItems = container->GetAllItems();

   // Adjust the vertical scroll to make sure all items can be scrolled through
   if (this->ItemSelectorSide == side) {
      *viewOffset = itemSelectorIndex - (maxItemsToShow / 2);
   }

   if (*viewOffset + maxItemsToShow > containerItems.size() + 1) {
      *viewOffset = ((int)containerItems.size() + 1) - maxItemsToShow;
   }

   if (*viewOffset < 0) {
      *viewOffset = 0;
   }



   y += 1;

   for (auto itemIndexOffset = 0; itemIndexOffset < maxItemsToShow; itemIndexOffset++) {
      if (itemIndexOffset + *viewOffset == containerItems.size()) {
         if (this->ItemSelectorSide == side && itemSelectorIndex == itemIndexOffset + *viewOffset) {
            // TODO: We need a way to draw a "selector" without rendering an actual button...
            Screen::Get().WriteButton(x + 2, y, width - 3, "", true);
         }
         break;
      }

      auto containerItem = containerItems[itemIndexOffset + *viewOffset];
      auto lineTextColor = Color::White;

      if ((this->ItemSelectorSide == side) && (itemSelectorIndex == itemIndexOffset + *viewOffset)) {
         lineTextColor = Color::Inverse(Color::White);

         // TODO: We need a way to draw a "selector" without rendering an actual button...
         Screen::Get().WriteButton(x + 2, y, width - 3, "", true);
      }

      Screen::Get().WriteText(x + 2, y, containerItem->GetName(), lineTextColor);
      if (SelectedItem == containerItem) {
         Screen::Get().WriteText(x + 1, y, "*", Color::BrightGreen);
      }

      auto obtainableInterface = containerItem->GetInterface<Obtainable>(ItemInterfaceType::Obtainable);
      if (obtainableInterface->GetIsStackable()) {
         Screen::Get().WriteText(x + width - 5, y, std::to_string(containerItem->GetCount()), lineTextColor);
      }

      y++;
   }
}

void ItemTransferDialog::DrawSelectedItemDescription()
{
   int* selectorIndex;
   ItemContainerPtr itemContainer;

   if (this->ItemSelectorSide == eItemDialogSide::LeftSide) {
      selectorIndex = &this->FirstItemSelectorIndex;
      itemContainer = this->FirstContainer;
   }
   else {
      selectorIndex = &this->SecondItemSelectorIndex;
      itemContainer = this->SecondContainer;
   }

   if (*selectorIndex == itemContainer->GetAllItems().size()) {
      return;
   }

   Screen::Get().WriteWindow(3, Screen::Get().GetHeight() - 5, Screen::Get().GetWidth() - 6, 3);
   Screen::Get().WriteText(4, Screen::Get().GetHeight() - 4, itemContainer->GetAllItems()[*selectorIndex]->GetDescription());
}

void ItemTransferDialog::MoveSelectorInWindow(int adjustment)
{
   int* selectorIndex;
   ItemContainerPtr itemContainer;

   if (this->ItemSelectorSide == eItemDialogSide::LeftSide) {
      selectorIndex = &this->FirstItemSelectorIndex;
      itemContainer = this->FirstContainer;
   }
   else {
      selectorIndex = &this->SecondItemSelectorIndex;
      itemContainer = this->SecondContainer;
   }

   if (itemContainer->GetAllItems().size() == 0) {
      *selectorIndex = 0;
      return;
   }
   *selectorIndex += adjustment;

   if (*selectorIndex < 0) {
      *selectorIndex = (int)itemContainer->GetAllItems().size();
   }

   if (*selectorIndex > itemContainer->GetAllItems().size()) {
      *selectorIndex = 0;
   }
}

void ItemTransferDialog::SwitchSelectedWindow()
{
   this->ItemSelectorSide = (this->ItemSelectorSide == eItemDialogSide::LeftSide)
      ? eItemDialogSide::RightSide
      : eItemDialogSide::LeftSide;
}

void ItemTransferDialog::HandleActionButtonPressed()
{
   int* selectorIndex;
   ItemContainerPtr itemContainer;

   if (this->ItemSelectorSide == eItemDialogSide::LeftSide) {
      selectorIndex = &this->FirstItemSelectorIndex;
      itemContainer = this->FirstContainer;
   }
   else {
      selectorIndex = &this->SecondItemSelectorIndex;
      itemContainer = this->SecondContainer;
   }

   if (nullptr == this->SelectedItem) {
      // They are trying to select something

      if (*selectorIndex == itemContainer->GetAllItems().size()) {
         // But there's nothing here to select
         return;
      }

      // They have selected an item!
      this->SelectedItem = itemContainer->GetAllItems()[*selectorIndex];

      return;
   }

   if (*selectorIndex >= itemContainer->GetAllItems().size()) {
      // They are either moving the item to the end of a stack, or swapping to the other stack
      auto itemToMove = Item::Clone(this->SelectedItem);
      this->RemoveItemFromEitherContainer(this->SelectedItem);
      this->SelectedItem = nullptr;
      this->AddItemToEndOfCurrentContainer(itemToMove);
      return;
   }

   auto itemToSelect = itemContainer->GetAllItems()[*selectorIndex];
   if (this->SelectedItem->GetName() != itemToSelect->GetName()) {
      // They are swapping items
      this->SwapItems(this->SelectedItem, itemToSelect);
      this->SelectedItem = nullptr;
      return;
   }

   if ((this->SelectedItem == itemToSelect) && (this->SelectedItem->GetCount() == 1)) {
      // They selected an unsplitable item, nothing to do here...
      this->SelectedItem = nullptr;
      return;
   }

   if (this->SelectedItem == itemToSelect) {
      // They are splitting an item
      this->SplitItem(this->SelectedItem);
      this->SelectedItem = nullptr;
      return;
   }

   // They are combining an item
   this->CombineItems(this->SelectedItem, itemToSelect);
   this->SelectedItem = nullptr;
   
}

void ItemTransferDialog::RemoveItemFromEitherContainer(ItemPtr item)
{
   this->FirstContainer->RemoveItem(item);
   this->SecondContainer->RemoveItem(item);
}

void ItemTransferDialog::AddItemToEndOfCurrentContainer(ItemPtr item)
{
   auto container = (this->ItemSelectorSide == eItemDialogSide::LeftSide)
      ? this->FirstContainer
      : this->SecondContainer;

   container->AddItem(item, MOVE_AMOUNT_EVERYTHING, true);
}

void ItemTransferDialog::SwapItems(ItemPtr itemA, ItemPtr itemB)
{
   auto itemToMoveA = Item::Clone(itemA);
   auto itemToMoveB = Item::Clone(itemB);

   this->FirstContainer->SwapItem(itemA, itemToMoveB);
   this->SecondContainer->SwapItem(itemA, itemToMoveB);
   this->FirstContainer->SwapItem(itemB, itemToMoveA);
   this->SecondContainer->SwapItem(itemB, itemToMoveA);
}

void ItemTransferDialog::SplitItem(ItemPtr item)
{
   auto container = (this->ItemSelectorSide == eItemDialogSide::LeftSide)
      ? this->FirstContainer
      : this->SecondContainer;

   container->SplitItem(item);
}

void ItemTransferDialog::CombineItems(ItemPtr source, ItemPtr dest)
{
   this->FirstContainer->CombineItems(source, dest);
   this->SecondContainer->CombineItems(source, dest);
}
