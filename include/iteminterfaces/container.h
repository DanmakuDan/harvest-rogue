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
#include "tickevents.h"
#include "interactable.h"
#include "itemcontainer.h"
#include <memory>


class Container : 
   public IItemInterface, 
   public IInteractable,
   public IItemContainer {

   Container();
   Container(Container const &) {};
public:
   ~Container();

   // IItemInterface
   Container* Clone() const override;
   ItemInterfaceType::ItemInterfaceType GetInterfaceType() override;
   static std::shared_ptr<Container> Deserialize(picojson::value serializedValue);

   // IInteractable
   void Interact(ItemPtr sourceItem) override;

   // IItemContainer
   ItemListPtr GetAllItems() override;
   ItemContainerPtr AsItemContainer() override;
   void AddItem(ItemPtr item, int count, bool dontStack) override;
   void RemoveItem(ItemPtr item, int count = -1) override;
   void SwapItem(ItemPtr itemA, ItemPtr itemB) override;
   void SplitItem(ItemPtr item) override;
   void CombineItems(ItemPtr source, ItemPtr dest) override;

   // INameable
   std::string GetName() override;

private:
   ItemListPtr Contents;
};

#endif //HARVEST_ROGUE_CONTAINER_H
