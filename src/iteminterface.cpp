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

#include "iteminterface.h"
#include "durable.h"
#include "tillingtool.h"
#include "choppingtool.h"

std::shared_ptr<IItemInterface> IItemInterface::Deserialize(ItemInterfaceType::ItemInterfaceType interfaceType, picojson::value serializedValue)
{
   switch (interfaceType) {

   case ItemInterfaceType::ChoppingTool:
      return ChoppingTool::Deserialize(serializedValue);

   case ItemInterfaceType::Durable:
      return Durable::Deserialize(serializedValue);

   case ItemInterfaceType::TillingTool:
      return TillingTool::Deserialize(serializedValue);

   default:
      return nullptr;
   }
}
