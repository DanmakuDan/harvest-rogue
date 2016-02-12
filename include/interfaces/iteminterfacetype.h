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

#ifndef HARVEST_ROGUE_ITEMINTERFACETYPE_H
#define HARVEST_ROGUE_ITEMINTERFACETYPE_H

#include <string>
#include <map>

namespace ItemInterfaceType {
   enum ItemInterfaceType {
      Unknown = -1,
      ChoppingTool = 0,
      TillingTool,
      Durable,
      Obtainable,
      DropsLoot,
      Choppable
   };

   static ItemInterfaceType FromString(std::string itemInterfaceType) {
      if (itemInterfaceType == "choppingTool") return ChoppingTool;
      if (itemInterfaceType == "durable") return Durable;
      if (itemInterfaceType == "tillingTool") return TillingTool;
      if (itemInterfaceType == "obtainable") return Obtainable;
      if (itemInterfaceType == "dropsLoot") return DropsLoot;
      if (itemInterfaceType == "choppable") return Choppable;

      return Unknown;
   }
}

#endif //HARVEST_ROGUE_ITEMINTERFACETYPE_H
