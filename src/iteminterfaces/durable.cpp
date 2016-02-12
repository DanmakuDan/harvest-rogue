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

#include "durable.h"
#include "..\..\include\iteminterfaces\durable.h"

Durable::Durable()
{
}

Durable::~Durable()
{
}

std::shared_ptr<Durable> Durable::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<Durable>(new Durable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "durability") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value != (unsigned int)value) {
            throw;
         }

         result->SetDurability(value);
         continue;
      }

      if (item.first == "repairable") {
         if (!item.second.is<bool>()) {
            throw;
         }

         auto value = item.second.get<bool>();

         result->SetRepairable(value);
         continue;
      }

      throw;
   }

   return result;
}

void Durable::SetDurability(int durability)
{
   this->Durability = durability;
}

int Durable::GetDurability()
{
   return this->Durability;
}

void Durable::SetRepairable(bool repairable)
{
   this->Repairable = repairable;
}

bool Durable::GetRepairable()
{
   return this->Repairable;
}

ItemInterfaceType::ItemInterfaceType Durable::GetInterfaceType()
{
   return ItemInterfaceType::Durable;
}
