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

#ifndef HARVEST_ROGUE_CHOPPABLE_H
#define HARVEST_ROGUE_CHOPPABLE_H


#include "iteminterface.h"
#include "choppingtool.h"
#include <memory>

class Choppable : public IItemInterface {
public:
   IItemInterface* Clone() const override;
private:
   Choppable();

   Choppable(Choppable const &src) {
      this->ChopPoints = src.ChopPoints;
   };
public:
   ~Choppable();
   static std::shared_ptr<Choppable> Deserialize(picojson::value serializedValue);

   int GetChopPoints() const;
   void SetChopPoints(int chopPoints);
   void Chop(ItemPtr sourceItem, ChoppingTool* choppingTool);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
private:
   int ChopPoints;
};

#endif //HARVEST_ROGUE_CHOPPABLE_H
