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

#include "player.h"
#include "gamestate.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Reset() {
   this->WarpPlayer(0, 0);
}

int Player::GetPositionX() {
   return this->PositionX;
}

int Player::GetPositionY() {
   return this->PositionY;
}

void Player::WarpPlayer(int x, int y) {
   this->PositionX = x;
   this->PositionY = y;
}

void Player::WalkPlayer(eDirection direction) {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   switch (direction) {
      case DirectionUp:
         if (this->PositionX == 0)
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PositionX, this->PositionY - 1), Walkable)) {
            GameState::Get().AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PositionX, this->PositionY - 1).Name.c_str());
            return;
         }
         this->PositionY--;
         break;

      case DirectionDown:
         if (this->PositionY == (currentLandmark->GetHeight() - 1))
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PositionX, this->PositionY + 1), Walkable)) {
            GameState::Get().AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PositionX, this->PositionY + 1).Name.c_str());
            return;
         }
         this->PositionY++;
         break;

      case DirectionLeft:
         if (this->PositionX == 0)
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PositionX - 1, this->PositionY), Walkable)) {
            GameState::Get().AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PositionX - 1, this->PositionY).Name.c_str());
            return;
         }
         this->PositionX--;
         break;

      case DirectionRight:
         if (this->PositionX == (currentLandmark->GetWidth() - 1))
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PositionX + 1, this->PositionY), Walkable)) {
            GameState::Get().AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PositionX + 1, this->PositionY).Name.c_str());
            return;
         }
         this->PositionX++;
         break;
   }

}

std::shared_ptr<ITool> Player::GetCurrentTool() {
   return std::shared_ptr<ITool>(this->GetCurrentTool());
}

std::vector<std::shared_ptr<IProp>> Player::GetInventory() {
   return this->Inventory;
}

void Player::SpawnIntoInventory(std::shared_ptr<IProp> prop) {
   this->Inventory.push_back(std::shared_ptr<IProp>(prop));
}
