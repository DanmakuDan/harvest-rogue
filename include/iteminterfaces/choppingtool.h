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

#ifndef HARVEST_ROGUE_CHOPPINGTOOL_H
#define HARVEST_ROGUE_CHOPPINGTOOL_H

#include "iteminterface.h"
#include <memory>

class ChoppingTool : public IItemInterface {
private:
   ChoppingTool();

   ChoppingTool(ChoppingTool const &) { };

   ChoppingTool &operator=(ChoppingTool const &) { };
public:
   ~ChoppingTool();
   static std::shared_ptr<ChoppingTool> Deserialize(picojson::value serializedValue);

   int GetStrength();
   void SetStrength(int strength);
   int GetFatigue();
   void SetFatigue(int fatigue);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
private:
   int Strength;
   int Fatigue;
};

#endif //HARVEST_ROGUE_CHOPPINGTOOL_H
