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


#ifndef HARVEST_ROGUE_DROPSLOOT_H
#define HARVEST_ROGUE_DROPSLOOT_H


#include "iteminterface.h"
#include "choppingtool.h"
#include <memory>
#include <string>
#include <list>

struct DropsLootItem {
   std::string ItemName;
   double Chance;
   int AmountMin;
   int AmountMax;
};

class DropsLoot : public IItemInterface {
public:
   IItemInterface* Clone() const { return new DropsLoot(*this); }
private:
   DropsLoot();

   DropsLoot(DropsLoot const &src) { 
      this->Loot = src.Loot;
   };

   DropsLoot &operator=(DropsLoot const &) { };
public:
   ~DropsLoot();
   static std::shared_ptr<DropsLoot> Deserialize(picojson::value serializedValue);

   void DropLoot(int x, int y);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
private:

   std::list<DropsLootItem> Loot;
};

#endif //HARVEST_ROGUE_DROPSLOOT_H
