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

#include "axe.h"
#include "gamestate.h"
#include "actiondirectiondialog.h"
#include "player.h"
#include "choppable.h"

Axe::Axe() {

}

std::string Axe::GetName() {
   return "Hand Axe";
}

std::string Axe::GetDescription() {
   return "A tool for chopping wood";
}

TileType::TileType Axe::GetTileType() {
   return TileType::Axe;
}

bool Axe::Takeable() {
   return true;
}

bool Axe::IsUsable() {
   return true;
}

void Axe::Use() {
   GameState::Get().PushDialog(ActionDirectionDialog::Construct(this->shared_from_this()));
}

void Axe::Use(Direction::Direction direction) {
   auto landmark = GameState::Get().GetCurrentLandmark();

   int targetX, targetY;
   switch(direction) {
      case Direction::Up:
         targetX = Player::Get().GetPositionX();
         targetY = Player::Get().GetPositionY() - 1;
         break;
      case Direction::Down:
         targetX = Player::Get().GetPositionX();
         targetY = Player::Get().GetPositionY() + 1;
         break;
      case Direction::Left:
         targetX = Player::Get().GetPositionX() - 1;
         targetY = Player::Get().GetPositionY();
         break;
      case Direction::Right:
         targetX = Player::Get().GetPositionX() + 1;
         targetY = Player::Get().GetPositionY();
         break;
   }

   auto prop = landmark->GetProp(targetX, targetY);
   if (prop == nullptr) {
      GameState::Get().AddLogMessage("There is nothing to chop.");
      return;
   }

   auto choppable = std::dynamic_pointer_cast<IChoppable>(prop);
   if (choppable == nullptr) {
      GameState::Get().AddLogMessageFmt("You cannot chop the %s.", prop->GetName().c_str());
      return;
   }

   GameState::Get().AddLogMessageFmt("You chop at the %s.", prop->GetName().c_str());

   choppable->Chop(10);
}
