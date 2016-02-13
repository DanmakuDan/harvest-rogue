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

#ifndef HARVEST_ROGUE_HARVESTABLE_H
#define HARVEST_ROGUE_HARVESTABLE_H


#include "iteminterface.h"
#include <memory>

class Harvestable : public IItemInterface {
public:
   IItemInterface* Clone() const { return new Harvestable(*this); }
private:
   Harvestable();

   Harvestable(Harvestable const &src) {
      this->YieldItem = src.YieldItem;
      this->YieldMinimum = src.YieldMinimum;
      this->YieldMaximum = src.YieldMaximum;
   };
public:
   ~Harvestable();
   static std::shared_ptr<Harvestable> Deserialize(picojson::value serializedValue);

   std::string GetYieldItem();
   void SetYieldItem(std::string yieldItem);
   int GetYieldMinimum();
   void SetYieldMinimum(int yieldMinimum);
   int GetYieldMaximum();
   void SetYieldMaximum(int yieldMaximum);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
private:
   std::string YieldItem;
   int YieldMinimum;
   int YieldMaximum;
};

#endif //HARVEST_ROGUE_HARVESTABLE_H
