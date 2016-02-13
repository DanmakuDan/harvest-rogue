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

#ifndef HARVEST_ROGUE_ITEM_H
#define HARVEST_ROGUE_ITEM_H

#include <map>
#include <memory>
#include <list>
#include "iteminterface.h"
#include "tiles.h"
#include "itemcategory.h"
#include "direction.h"

class Item : public std::enable_shared_from_this<Item> {
public:
   static Item Clone(const Item& source);
   Item();
   std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> GetInterfaces();
   bool HasInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType);
   void AddInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType, std::shared_ptr<IItemInterface> itemInterface);
   void RemoveInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType);
   void SetName(std::string name);
   void SetDescription(std::string description);
   void SetSurfaceAttributes(SurfaceAttribute::SurfaceAttribute surfaceAttributes);
   void SetColorCode(Color::Color colorCode);
   void SetCharacterCode(char characterCode);
   void SetGfxTileCode(int gfxTileCode);
   std::list<ItemCategory::ItemCategory> GetItemCategories();
   void SetItemCategories(std::list<ItemCategory::ItemCategory> itemCategories);
   int GetCount();
   void SetCount(int count);

   std::string GetName();
   std::string GetDescription();
   SurfaceAttribute::SurfaceAttribute GetSurfaceAttributes();
   char GetCharacterCode();
   int GetGfxTileCode();
   Color::Color GetColorCode();
   bool Takeable();
   bool IsUsable();
   bool IsEquippable();
   void Use();
   void Use(Direction::Direction direction);
   void Destruct();
   void RemoveOne();
   void NotifyItemEquipped();
   void NotifyItemUnequiupped();

   template<class T>
   inline std::shared_ptr<T> GetInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType) {
      return std::dynamic_pointer_cast<T>(std::shared_ptr<IItemInterface>(this->ItemInterfaces[itemInterfaceType]));
   }
private:
   std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> ItemInterfaces;
   std::string Name;
   std::string Description;
   Color::Color ColorCode;
   char CharacterCode;
   int GfxTileCode;
   SurfaceAttribute::SurfaceAttribute SurfaceAttributes;
   std::list<ItemCategory::ItemCategory> ItemCategories;
   int Count;
};

#endif //HARVEST_ROGUE_ITEM_H
