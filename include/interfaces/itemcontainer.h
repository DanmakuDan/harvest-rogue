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

#ifndef HARVEST_ROGUE_ITEMCONTAINER_H
#define HARVEST_ROGUE_ITEMCONTAINER_H

#include <vector>
#include <memory>
#include "item.h"
#include "nameable.h"

class IItemContainer; // Foward declaration for the typedef
typedef IItemContainer* ItemContainerPtr;

#define MOVE_AMOUNT_EVERYTHING -1

// Declares that this object has the ability to contain items
class IItemContainer : 
   public INameable {
public:
   virtual ItemListPtr GetAllItems() = 0;
   virtual ItemContainerPtr AsItemContainer() = 0;
   virtual void AddItem(ItemPtr item, int count, bool dontStack) = 0;
   virtual void RemoveItem(ItemPtr item, int count = -1) = 0;
   virtual void SwapItem(ItemPtr itemA, ItemPtr itemB) = 0;
   virtual void SplitItem(ItemPtr item) = 0;
   virtual void CombineItems(ItemPtr source, ItemPtr dest) = 0;

   virtual ~IItemContainer() {}
};

#endif //HARVEST_ROGUE_ITEMCONTAINER_H
