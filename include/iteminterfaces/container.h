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

#ifndef HARVEST_ROGUE_CONTAINER_H
#define HARVEST_ROGUE_CONTAINER_H


#include "iteminterface.h"
#include "crops.h"
#include "tickevents.h"
#include "interactable.h"
#include "itemcontainer.h"
#include <memory>


class Container : 
   public IItemInterface, 
   public IInteractable,
   public IItemContainer {
public:
   Container* Clone() const { return new Container(*this); }
private:
   Container();

   Container(Container const &src) { };
public:
   ~Container();
   static std::shared_ptr<Container> Deserialize(picojson::value serializedValue);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();

   // IInteractable
   virtual void Interact(ItemPtr sourceItem);

   // IItemContainer
   virtual ItemListPtr GetAllItems();
   virtual ItemContainerPtr AsItemContainer();
   virtual void AddItem(ItemPtr item, int count, bool dontStack);
   virtual void RemoveItem(ItemPtr item, int count = -1);
   virtual void SwapItem(ItemPtr itemA, ItemPtr itemB);
   virtual void SplitItem(ItemPtr item);
   virtual void CombineItems(ItemPtr source, ItemPtr dest);

   // INameable
   virtual std::string GetName();

private:
   ItemListPtr Contents;
};

#endif //HARVEST_ROGUE_CONTAINER_H
