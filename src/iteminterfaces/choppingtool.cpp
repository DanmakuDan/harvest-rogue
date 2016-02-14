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

ChoppingTool::ChoppingTool()
{
}

ChoppingTool::~ChoppingTool()
{
}

std::shared_ptr<ChoppingTool> ChoppingTool::Deserialize(picojson::value serializedValue)
{
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

int ChoppingTool::GetStrength()
{
   return this->Strength;
}

void ChoppingTool::SetStrength(int strength)
{
   this->Strength = strength;
}

int ChoppingTool::GetFatigue()
{
   return this->Fatigue;
}

void ChoppingTool::SetFatigue(int fatigue)
{
   this->Fatigue = fatigue;
}

void ChoppingTool::Chop(std::shared_ptr<Item> sourceItem)
{
   GameState::Get().PushDialog(ActionDirectionDialog::Construct(sourceItem));
}

void ChoppingTool::Chop(std::shared_ptr<Item> sourceItem, Direction::Direction direction)
{
   auto landmark = GameState::Get().GetCurrentLandmark();

   int targetX, targetY;
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

   choppable->Chop(std::shared_ptr<Item>(item), this);
}

ItemInterfaceType::ItemInterfaceType ChoppingTool::GetInterfaceType()
{
   return ItemInterfaceType::ChoppingTool;
}

void ChoppingTool::Use(std::shared_ptr<Item> sourceItem)
{
   this->Chop(sourceItem);
}

void ChoppingTool::Use(std::shared_ptr<Item> sourceItem, Direction::Direction direction)
{
   this->Chop(sourceItem, direction);
}

void ChoppingTool::OnItemEquipped(std::shared_ptr<Item> sourceItem)
{
}

void ChoppingTool::OnItemUnequipped(std::shared_ptr<Item> sourceItem)
{
}
