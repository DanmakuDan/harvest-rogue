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

#include "dropsloot.h"
#include "gamestate.h"
#include "obtainable.h"
#include <random>

std::default_random_engine generator;

DropsLoot::DropsLoot()
{
}

DropsLoot::~DropsLoot()
{
}

std::shared_ptr<DropsLoot> DropsLoot::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<DropsLoot>(new DropsLoot());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {
      DropsLootItem lootItem;
      lootItem.ItemName = item.first;

      if (!item.second.is<picojson::object>()) {
         throw;
      }

      auto details = item.second.get<picojson::object>();
      for (auto detailItem : details) {
         if (detailItem.first == "chance") {

            if (!detailItem.second.is<double>()) {
               throw;
            }

            lootItem.Chance = detailItem.second.get<double>();
            continue;
         }

         if (detailItem.first == "amount") {

            if (!detailItem.second.is<picojson::object>()) {
               throw;
            }

            auto amountDetails = detailItem.second.get<picojson::object>();
            for (auto amountItem : amountDetails) {
               if (amountItem.first == "min") {
                  if (!amountItem.second.is<double>()) {
                     throw;
                  }
                  auto amount = amountItem.second.get<double>();
                  if (amount != (unsigned int)amount) {
                     throw;
                  }
                  lootItem.AmountMin = (int)amount;
                  continue;
               }

               if (amountItem.first == "max") {
                  if (!amountItem.second.is<double>()) {
                     throw;
                  }
                  auto amount = amountItem.second.get<double>();
                  if (amount != (unsigned int)amount) {
                     throw;
                  }
                  lootItem.AmountMax = (int)amount;
                  continue;
               }

               throw;
            }
            continue;
         }

         throw;
      }

      result->Loot.push_back(lootItem);
   }

   return result;
}

void DropsLoot::DropLoot(int x, int y)
{
   if (this->Loot.size() > 1) {
      GameState::Get().AddLogMessage("Multi-item loot drops are not currently implemented.");
      return;
   }

   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto lootItem = this->Loot.front();
   auto item = GameState::Get().GetItemFromItemDatabase(lootItem.ItemName);
   std::uniform_int_distribution<int> distribution(lootItem.AmountMin, lootItem.AmountMax);
   auto amount = distribution(generator);

   if (amount == 0) {
      return;
   }
   if (!item->HasInterface(ItemInterfaceType::Obtainable)) {
      if (amount > 1) {
         GameState::Get().AddLogMessage("Only items that have an obtainable interface can spawn more than 1 at a time.");
         return;
      }
      currentLandmark->AddItem(x, y, item);
      return;
   }

   item->SetCount(amount);

   currentLandmark->AddItem(x, y, item);

}

ItemInterfaceType::ItemInterfaceType DropsLoot::GetInterfaceType()
{
   return ItemInterfaceType::DropsLoot;
}
