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

#include "harvestable.h"
#include "gamestate.h"

Harvestable::Harvestable()
{
}

Harvestable::~Harvestable()
{
}

std::shared_ptr<Harvestable> Harvestable::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<Harvestable>(new Harvestable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "yieldItem") {
         if (!item.second.is<std::string>()) {
            throw;
         }

         auto value = item.second.get<std::string>();
         result->SetYieldItem(value);
         continue;
      }

      if (item.first == "yieldMin") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value != (unsigned int)value) {
            throw;
         }
         result->SetYieldMinimum((int)value);
         continue;
      }

      if (item.first == "yieldMax") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value != (unsigned int)value) {
            throw;
         }
         result->SetYieldMaximum((int)value);
         continue;
      }

      throw;
   }

   return result;
}

std::string Harvestable::GetYieldItem()
{
   return this->YieldItem;
}

void Harvestable::SetYieldItem(std::string yieldItem)
{
   this->YieldItem = YieldItem;
}

int Harvestable::GetYieldMinimum()
{
   return this->YieldMinimum;
}

void Harvestable::SetYieldMinimum(int yieldMinimum)
{
   this->YieldMinimum = YieldMinimum;
}

int Harvestable::GetYieldMaximum()
{
   return this->YieldMaximum;
}

void Harvestable::SetYieldMaximum(int yieldMaximum)
{
   this->YieldMaximum = yieldMaximum;
}

ItemInterfaceType::ItemInterfaceType Harvestable::GetInterfaceType()
{
   return ItemInterfaceType::Harvestable;
}
