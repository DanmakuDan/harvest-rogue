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

#ifndef HARVEST_ROGUE_DURABLE_H
#define HARVEST_ROGUE_DURABLE_H

#include "iteminterface.h"
#include <memory>

class Durable : public IItemInterface {
private:
   Durable();

   Durable(Durable const &) { };

   Durable &operator=(Durable const &) { };
public:
   ~Durable();
   static std::shared_ptr<Durable> Deserialize(picojson::value serializedValue);

   void SetDurability(int durability);
   int GetDurability();

   void SetRepairable(bool repairable);
   bool GetRepairable();

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
private:
   int Durability;
   bool Repairable;
};

#endif //HARVEST_ROGUE_DURABLE_H
