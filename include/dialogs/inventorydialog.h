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

#ifndef HARVEST_ROGUE_INVENTORYDIALOG_H
#define HARVEST_ROGUE_INVENTORYDIALOG_H

#include <memory>
#include "dialog.h"
#include "itemcontainer.h"

#define INVENTORY_DIALOG_WIDTH      40
#define INVENTORY_MAX_ITEMS_SHOWN   5

class InventoryDialog : public IDialog {
public:
   static std::shared_ptr<InventoryDialog> Construct(ItemContainerPtr itemContainer) {
      return std::shared_ptr<InventoryDialog>(new InventoryDialog(itemContainer));
   }
   virtual void OnKeyPress(int key);
   virtual void Render();
private:
   InventoryDialog(ItemContainerPtr itemContainer);
   int InventoryOffset;
   int SelectedInventoryItem;
   void ExecuteSelectedAction();

   ItemContainerPtr ItemContainer;
};


#endif //HARVEST_ROGUE_INVENTORYDIALOG_H
