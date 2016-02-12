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

class Item {
public:
   std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> GetInterfaces();
   std::shared_ptr<IItemInterface> GetInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType);
   void AddInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType, std::shared_ptr<IItemInterface> itemInterface);
   void RemoveInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType);
   std::string GetName();
   void SetName(std::string name);
   std::string GetDescription();
   void SetDescription(std::string description);
   SurfaceAttribute::SurfaceAttribute GetSurfaceAttributes();
   void SetSurfaceAttributes(SurfaceAttribute::SurfaceAttribute surfaceAttributes);
   void SetColorCode(int colorCode);
   int GetColorCode();
   void SetCharacterCode(char characterCode);
   char GetCharacterCode();
   void SetGfxTileCode(int gfxTileCode);
   int GetGfxTileCode();
   std::list<ItemCategory::ItemCategory> GetItemCategories();
   void SetItemCategories(std::list<ItemCategory::ItemCategory> itemCategories);
private:
   std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> ItemInterfaces;
   std::string Name;
   std::string Description;
   int ColorCode;
   char CharacterCode;
   int GfxTileCode;
   SurfaceAttribute::SurfaceAttribute SurfaceAttributes;
   std::list<ItemCategory::ItemCategory> ItemCategories;
};

#endif //HARVEST_ROGUE_ITEM_H
