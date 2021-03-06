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

#ifndef HARVEST_ROGUE_ITEMCATEGORY_H
#define HARVEST_ROGUE_ITEMCATEGORY_H

#include <string>

namespace ItemCategory {
   enum ItemCategory {
      Unknown,
      Container,
      Crop,
      Seed,
      Tool,
      CraftingMaterial
   };

   static ItemCategory FromString(std::string itemCategory) {
      if (itemCategory == "container") return Container;
      if (itemCategory == "crop") return Crop;
      if (itemCategory == "seed") return Seed;
      if (itemCategory == "tool") return Tool;
      if (itemCategory == "craftingMaterial") return CraftingMaterial;

      return Unknown;
   }

   static std::string GetDescription(ItemCategory itemCategory) {
      switch(itemCategory) {
         case Unknown: return "Unknown";
         case Container: return "Container";
         case Crop: return "Crop";
         case Seed: return "Seed";
         case Tool: return "Tool";
         case CraftingMaterial: return "Crafting Material";
         default: return "[Unknown]";
      }
   }
}

#endif //HARVEST_ROGUE_ITEMCATEGORY_H
