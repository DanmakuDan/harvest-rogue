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


class Craftable;
typedef std::shared_ptr<Craftable> CraftablePtr;
class Craftable
   : public IItemInterface {

private:
   Craftable();
   Craftable(Craftable const &) { };

public:
   ~Craftable();

   std::map<std::string, int> GetRequiredMaterials() const;
   void SetRequiredMaterial(std::string itemName, int amount);
   int GetRequiredMaterialAmount(std::string itemName);
   int GetSecondsToCraft() const;
   void SetSecondsToCraft(int value);

   // IItemInterface
   Craftable* Clone() const override;
   ItemInterfaceType::ItemInterfaceType GetInterfaceType() override;
   static std::shared_ptr<Craftable> Deserialize(picojson::value serializedValue);

private:
   std::map<std::string, int> RequiredMaterials;
   static void DeserializeMaterialsRequired(picojson::value serializedValue, CraftablePtr craftable);
   int SecondsToCraft;
};


#endif //HARVEST_ROGUE_CRAFTABLE_H
