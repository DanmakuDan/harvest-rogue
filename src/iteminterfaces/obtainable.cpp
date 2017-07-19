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

#include "obtainable.h"

IItemInterface* Obtainable::Clone() const { return new Obtainable(*this); }

Obtainable::Obtainable() :
   Stackable(false),
   MaxStackSize(0) {}

Obtainable::~Obtainable() {}

std::shared_ptr<Obtainable> Obtainable::Deserialize(picojson::value serializedValue) {
   auto result = std::shared_ptr<Obtainable>(new Obtainable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "stackable") {
         if (!item.second.is<bool>()) {
            throw;
         }

         auto value = item.second.get<bool>();
         result->SetIsStackable(value);
         continue;
      }

      if (item.first == "maxStackSize") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();

         if (value != static_cast<unsigned int>(value)) {
            throw;
         }

         result->SetMaxStackSize(static_cast<int>(value));
         continue;
      }

      throw;
   }

   return result;
}

bool Obtainable::GetIsStackable() const {
   return this->Stackable;
}

void Obtainable::SetIsStackable(bool stackable) {
   this->Stackable = stackable;
}

int Obtainable::GetMaxStackSize() const {
   return this->MaxStackSize;
}

void Obtainable::SetMaxStackSize(int maxStackSize) {
   this->MaxStackSize = maxStackSize;
}

ItemInterfaceType::ItemInterfaceType Obtainable::GetInterfaceType() {
   return ItemInterfaceType::Obtainable;
}
