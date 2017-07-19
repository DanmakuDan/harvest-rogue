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

#include "choppable.h"
#include "gamestate.h"

IItemInterface* Choppable::Clone() const { return new Choppable(*this); }

Choppable::Choppable() :
   ChopPoints(0) {}

Choppable::~Choppable() {}

std::shared_ptr<Choppable> Choppable::Deserialize(picojson::value serializedValue) {
   auto result = std::shared_ptr<Choppable>(new Choppable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "chopPoints") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         result->SetChopPoints(static_cast<int>(value));
         continue;
      }

      throw;
   }

   return result;
}

int Choppable::GetChopPoints() const {
   return this->ChopPoints;
}

void Choppable::SetChopPoints(int chopPoints) {
   this->ChopPoints = chopPoints;
}

void Choppable::Chop(ItemPtr sourceItem, ChoppingTool* choppingTool) {
   if (this->GetChopPoints() < choppingTool->GetStrength()) {
      sourceItem->Destruct(true);
      return;
   }

   this->SetChopPoints(this->GetChopPoints() - choppingTool->GetStrength());
}

ItemInterfaceType::ItemInterfaceType Choppable::GetInterfaceType() {
   return ItemInterfaceType::Choppable;
}
