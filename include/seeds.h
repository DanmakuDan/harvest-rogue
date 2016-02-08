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

#ifndef HARVEST_ROGUE_SEEDS_H
#define HARVEST_ROGUE_SEEDS_H

namespace SeedType {
   enum SeedType {
      Corn = 0,
      Potato,
      Wheat
   };

   static std::string GetName(SeedType seedType) {
      switch (seedType) {
         case Corn:   return "Corn";
         case Potato: return "Potato";
         case Wheat:  return "Wheat";
      }
   }
}

#endif //HARVEST_ROGUE_SEEDS_H
