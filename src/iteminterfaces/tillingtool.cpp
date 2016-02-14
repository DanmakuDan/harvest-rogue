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

#include "tillingtool.h"
#include "gamestate.h"
#include "player.h"

TillingTool::TillingTool()
{
}

TillingTool::~TillingTool()
{
}

std::shared_ptr<TillingTool> TillingTool::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<TillingTool>(new TillingTool());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "strength") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value != (unsigned int)value) {
            throw;
         }

         result->SetStrength((int)value);
         continue;
      }

      if (item.first == "fatigue") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();

         if (value != (unsigned int)value) {
            throw;
         }

         result->SetFatigue((int)value);
         continue;
      }

      throw;
   }

   return result;
}

int TillingTool::GetStrength()
{
   return this->Strength;
}

void TillingTool::SetStrength(int strength)
{
   this->Strength = strength;
}

int TillingTool::GetFatigue()
{
   return this->Fatigue;
}

void TillingTool::SetFatigue(int fatigue)
{
   this->Fatigue = fatigue;
}

void TillingTool::Till(std::shared_ptr<Item> sourceItem)
{
   auto landmark = GameState::Get().GetCurrentLandmark();
   if (landmark == nullptr) {
      return;
   }
   
   auto playerX = Player::Get().GetPositionX();
   auto playerY = Player::Get().GetPositionY();
   auto currentTile = landmark->GetTile(playerX, playerY);
   switch(currentTile.TileType) {
      case TileType::GrassTuft:
         landmark->SetTile(playerX, playerY, TileType::Grass);
         Player::Get().AdjustEnergy(this->GetFatigue());
         break;
      case TileType::Grass:
         landmark->SetTile(playerX, playerY, TileType::Tilled);
         Player::Get().AdjustEnergy(this->GetFatigue());
         break;
   }
}

ItemInterfaceType::ItemInterfaceType TillingTool::GetInterfaceType()
{
   return ItemInterfaceType::TillingTool;
}

void TillingTool::Use(std::shared_ptr<Item> sourceItem)
{
   this->Till(sourceItem);
}

void TillingTool::OnItemEquipped(std::shared_ptr<Item> sourceItem)
{
}

void TillingTool::OnItemUnequipped(std::shared_ptr<Item> sourceItem)
{
}
