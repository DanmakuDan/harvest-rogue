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

#include "tree.h"
#include "gamestate.h"

Tree::Tree() {
   this->Durability = 30;
}

std::string Tree::GetName() {
   return "Tree";
}

std::string Tree::GetDescription() {
   return "A simple tree.";
}

TileType::TileType Tree::GetTileType() {
   return TileType::Tree;
}

bool Tree::Takeable() {
   return false;
}

void Tree::Chop(int strength) {
   this->Durability -= strength;
   if (this->Durability > 0) {
      return;
   }

   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   int x, y;
   if (!currentLandmark->LocateProp(this->shared_from_this(), x, y)) {
      throw;
   }

   currentLandmark->RemoveProp(x, y);

   GameState::Get().AddLogMessageFmt("The %s has been felled!", this->GetName().c_str());

}
