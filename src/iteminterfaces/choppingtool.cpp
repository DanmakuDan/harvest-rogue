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

#include "choppingtool.h"
#include "actiondirectiondialog.h"
#include "gamestate.h"
#include "player.h"
#include "choppable.h"

IItemInterface* ChoppingTool::Clone() const { return new ChoppingTool(*this); }

ChoppingTool::ChoppingTool() :
   Strength(0), Fatigue(0) {}

ChoppingTool::~ChoppingTool() {}

std::shared_ptr<ChoppingTool> ChoppingTool::Deserialize(picojson::value serializedValue) {
   auto result = std::shared_ptr<ChoppingTool>(new ChoppingTool());

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

         result->SetStrength(static_cast<int>(value));
         continue;
      }

      if (item.first == "fatigue") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();

         result->SetFatigue(static_cast<int>(value));
         continue;
      }

      throw;
   }

   return result;
}

int ChoppingTool::GetStrength() const {
   return this->Strength;
}

void ChoppingTool::SetStrength(int strength) {
   this->Strength = strength;
}

int ChoppingTool::GetFatigue() const {
   return this->Fatigue;
}

void ChoppingTool::SetFatigue(int fatigue) {
   this->Fatigue = fatigue;
}

void ChoppingTool::Chop(ItemPtr sourceItem) {
   GameState::Get().PushDialog(ActionDirectionDialog::Construct(sourceItem));
}

void ChoppingTool::Chop(Direction::Direction direction) {
   auto landmark = GameState::Get().GetCurrentLandmark();

   auto targetX = 0, targetY = 0;
   switch (direction) {
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

   auto item = landmark->GetItem(targetX, targetY);
   if (item == nullptr || !item->HasInterface(ItemInterfaceType::Choppable)) {
      GameState::Get().AddLogMessage("There is nothing to chop.");
      return;
   }

   auto choppable = item->GetInterface<Choppable>(ItemInterfaceType::Choppable);
   if (choppable == nullptr) {
      GameState::Get().AddLogMessageFmt("You cannot chop the %s.", item->GetName().c_str());
      return;
   }

   GameState::Get().AddLogMessageFmt("You chop at the %s.", item->GetName().c_str());
   Player::Get().AdjustEnergy(this->GetFatigue());
   choppable->Chop(ItemPtr(item), this);
}

ItemInterfaceType::ItemInterfaceType ChoppingTool::GetInterfaceType() {
   return ItemInterfaceType::ChoppingTool;
}

void ChoppingTool::Use(ItemPtr sourceItem) {
   this->Chop(sourceItem);
}

void ChoppingTool::Use(ItemPtr sourceItem, Direction::Direction direction) {
   this->Chop(direction);
}

void ChoppingTool::OnItemEquipped(ItemPtr sourceItem) {}

void ChoppingTool::OnItemUnequipped(ItemPtr sourceItem) {}
