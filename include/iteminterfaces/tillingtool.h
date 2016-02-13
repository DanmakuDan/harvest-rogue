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

#ifndef HARVEST_ROGUE_TILLINGTOOL_H
#define HARVEST_ROGUE_TILLINGTOOL_H

#include <memory>
#include "iteminterface.h"
#include "item.h"

class TillingTool : public IItemInterface {
public:
   IItemInterface* Clone() const { return new TillingTool(*this); }
private:
   TillingTool();

   TillingTool(TillingTool const &src) {
      this->Strength = src.Strength;
      this->Fatigue = src.Fatigue;
   };

   TillingTool &operator=(TillingTool const &) { };
public:
   ~TillingTool();
   static std::shared_ptr<TillingTool> Deserialize(picojson::value serializedValue);

   int GetStrength();
   void SetStrength(int strength);
   int GetFatigue();
   void SetFatigue(int fatigue);

   void Till(std::shared_ptr<Item> sourceItem);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
private:
   int Strength;
   int Fatigue;
};

#endif //HARVEST_ROGUE_TILLINGTOOL_H
