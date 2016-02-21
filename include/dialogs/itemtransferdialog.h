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

#ifndef HARVEST_ROGUE_ITEMTRANSFERDIALOG_H
#define HARVEST_ROGUE_ITEMTRANSFERDIALOG_H

#include <memory>
#include "dialog.h"
#include "itemcontainer.h"

class ItemTransferDialog: public IDialog {
private:
	enum eItemDialogSide { LeftSide, RightSide };

public:
   static std::shared_ptr<ItemTransferDialog> Construct(ItemContainerPtr firstContainer, ItemContainerPtr secondContainer) {
      return std::shared_ptr<ItemTransferDialog>(new ItemTransferDialog(firstContainer, secondContainer));
   }
   virtual void OnKeyPress(int key);
   virtual void Render();
private:
   ItemTransferDialog(ItemContainerPtr firstContainer, ItemContainerPtr secondContainer);

   void DrawDialogHeader(int x, int y, int width, int height);
   void DrawItemDialog(int x, int y, int width, int height, eItemDialogSide side);
   void DrawSelectedItemDescription();

   void MoveSelectorInWindow(int adjustment);
   void SwitchSelectedWindow();
   void HandleActionButtonPressed();

   void RemoveItemFromEitherContainer(ItemPtr item);
   void AddItemToEndOfCurrentContainer(ItemPtr item);
   void SwapItems(ItemPtr itemA, ItemPtr itemB);
   void SplitItem(ItemPtr item);
   void CombineItems(ItemPtr source, ItemPtr dest);

   ItemContainerPtr FirstContainer;
   ItemContainerPtr SecondContainer;

   eItemDialogSide ItemSelectorSide;
   int FirstItemSelectorIndex;
   int FirstItemViewOffset;
   int SecondItemSelectorIndex;
   int SecondItemViewOffset;

   ItemPtr SelectedItem;
};

#endif //HARVEST_ROGUE_ITEMTRANSFERDIALOG_H
