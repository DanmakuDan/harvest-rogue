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

#include "growable.h"
#include "gamestate.h"

Growable::Growable()
{
}

Growable::~Growable()
{
}


std::shared_ptr<Growable> Growable::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<Growable>(new Growable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      /*if (item.first == "yieldItem") {
         if (!item.second.is<std::string>()) {
            throw;
         }

         auto value = item.second.get<std::string>();
         result->SetYieldItem(value);
         continue;
      }*/
   }

   return result;
}

ItemInterfaceType::ItemInterfaceType Growable::GetInterfaceType()
{
   return ItemInterfaceType::Growable;
}
