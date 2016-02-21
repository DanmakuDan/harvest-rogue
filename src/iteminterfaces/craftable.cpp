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

#include "craftable.h"

Craftable::Craftable() {

}

Craftable::~Craftable() {

}

ItemInterfaceType::ItemInterfaceType Craftable::GetInterfaceType() {
   return ItemInterfaceType::Craftable;
}

std::shared_ptr<Craftable> Craftable::Deserialize(picojson::value serializedValue) {
   auto result = std::shared_ptr<Craftable>(new Craftable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {
      if (item.first == "materialsRequired") {
         //Craftable::DeserializeMaterialsRequired(item.second, result);
         continue;
      }

      throw;
   }

   return result;
}

std::map<std::string, int> Craftable::GetRequiredMaterials() {
   return this->RequiredMaterials;
}

void Craftable::SetRequiredMaterial(std::string itemName, int amount) {
   this->RequiredMaterials[itemName] = amount;
}

int Craftable::GetRequiredMaterialAmount(std::string itemName) {
   if (this->RequiredMaterials.find(itemName) == this->RequiredMaterials.end()) {
      return 0;
   }

   return this->RequiredMaterials[itemName];
}

void Craftable::DeserializeMaterialsRequired(picojson::value serializedValue, Craftable *craftable) {
   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {
      auto itemName = item.first;
      for (auto itemDetails : item.second.get<picojson::object>()) {
         if (itemDetails.first == "amount") {

            if (!itemDetails.second.is<double>()) {
               throw;
            }

            craftable->SetRequiredMaterial(itemName, (int)itemDetails.second.get<double>());
            continue;
         }

         throw;
      }
   }
}
