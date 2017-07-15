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
#include <vector>
#include "iteminterface.h"
#include "tiles.h"
#include "itemcategory.h"
#include "direction.h"
#include "nameable.h"


// The base class for all items in the system.
// This object specifically tracks all item interfaces for this item, as well as the number of items.
class Item;
typedef std::shared_ptr<Item> ItemPtr;
class Item : 
   public INameable,
   public std::enable_shared_from_this<Item> {
public:
   static ItemPtr Clone(const ItemPtr source);
   Item();
   std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> GetInterfaces() const;
   bool HasInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType);
   void AddInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType, std::shared_ptr<IItemInterface> itemInterface);
   void SetName(std::string name);
   void SetDescription(std::string description);
   void SetSurfaceAttributes(SurfaceAttribute::SurfaceAttribute surfaceAttributes);
   void SetColorCode(Color::Color colorCode);
   void SetCharacterCode(char characterCode);
   void SetGfxTileCode(int gfxTileCode);
   std::list<ItemCategory::ItemCategory> GetItemCategories() const;
   void SetItemCategories(std::list<ItemCategory::ItemCategory> itemCategories);
   int GetCount() const;
   void SetCount(int count);

   std::string GetDescription() const;
   SurfaceAttribute::SurfaceAttribute GetSurfaceAttributes() const;
   char GetCharacterCode() const;
   int GetGfxTileCode() const;
   Color::Color GetColorCode() const;
   bool IsTakeable();
   bool IsUsable();
   void Use();
   void Use(Direction::Direction direction);
   bool IsEquippable();
   bool IsInteractable() const;
   void Interact();
   void Destruct(bool dropLoot);
   void RemoveOne();
   void NotifyItemEquipped();
   void NotifyItemUnequipped();

   template<class T>
   inline std::shared_ptr<T> GetInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType) {
      return std::dynamic_pointer_cast<T>(std::shared_ptr<IItemInterface>(this->ItemInterfaces[itemInterfaceType]));
   }

   // INameable
   std::string GetName() override;

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

typedef std::vector<ItemPtr> ItemListPtr;

#endif //HARVEST_ROGUE_ITEM_H
