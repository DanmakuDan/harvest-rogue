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

#include <tiles.h>
#include "seedbag.h"
#include "gamestate.h"
#include "player.h"

SeedBag::SeedBag(SeedType::SeedType seedType, int numberOfSeeds) {
   this->SeedType = seedType;
   this->NumberOfSeeds = numberOfSeeds;
}

std::string SeedBag::GetName() {
   std::stringstream result;
   result << SeedType::GetName(this->SeedType);
   result << " seed bag";
   return result.str();
}

std::string SeedBag::GetDescription() {
   std::stringstream result;
   result << this->NumberOfSeeds;
   result << " ";
   result << SeedType::GetName(this->SeedType);
   result << " seed";
   if (this->NumberOfSeeds != 1) {
      result << "s";
   }
   result << " remain.";
   return result.str();
}

bool SeedBag::IsUsable() {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto tile = currentLandmark->GetTile(Player::Get().GetPositionX(), Player::Get().GetPositionY());

   if (tile.TileType != TileType::Tilled) {
      GameState::Get().AddLogMessage("You can only plant seeds on tilled ground.");
      return false;
   }

   auto prop = currentLandmark->GetProp(Player::Get().GetPositionX(), Player::Get().GetPositionY());
   if (prop != nullptr) {
      GameState::Get().AddLogMessage("There is something already here.");
      return false;
   }

   if (this->NumberOfSeeds <= 0) {
      GameState::Get().AddLogMessage("There are no more seeds in this bag.");
      return false;
   }

   return true;
}

TileType::TileType SeedBag::GetTileType() {
   return TileType::SeedBag;
}

void SeedBag::Use() {
   if (this->NumberOfSeeds <= 0) {
      return;
   }
   this->NumberOfSeeds--;
}

bool SeedBag::Takeable() {
   return true;
}
