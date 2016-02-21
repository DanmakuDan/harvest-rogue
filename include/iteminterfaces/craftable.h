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

#ifndef HARVEST_ROGUE_CRAFTABLE_H
#define HARVEST_ROGUE_CRAFTABLE_H

#include "iteminterface.h"
#include "picojson.h"
#include <map>

class Craftable
   : public IItemInterface {

private:
   Craftable();
   Craftable(Craftable const &src) { };

public:
   ~Craftable();

   // IItemInterface
   Craftable* Clone() const { return new Craftable(*this); }
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
   static std::shared_ptr<Craftable> Deserialize(picojson::value serializedValue);

   std::map<std::string, int> GetRequiredMaterials();
   void SetRequiredMaterial(std::string itemName, int amount);
   int GetRequiredMaterialAmount(std::string itemName);

private:
   std::map<std::string, int> RequiredMaterials;
   static void DeserializeMaterialsRequired(picojson::value serializedValue, Craftable *craftable);
};


#endif //HARVEST_ROGUE_CRAFTABLE_H
