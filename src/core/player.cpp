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

#include <memory>
#include <tiles.h>
#include "player.h"
#include "gamestate.h"
#include "interactiondirectiondialog.h"
#include "interactable.h"

Player::Player() {
   this->Energy = ENERGY_MAX;
   this->IsSleeping = false;
}

Player::~Player() {

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

void Player::WalkPlayer(Direction::Direction direction) {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto newX = 0, newY = 0;
   switch (direction) {
      case Direction::Up:
         if (this->PositionX == 0)
            return;
         newX = this->GetPositionX();
         newY = this->GetPositionY() - 1;
         break;

      case Direction::Down:
         if (this->PositionY == (currentLandmark->GetHeight() - 1))
            return;
         newX = this->GetPositionX();
         newY = this->GetPositionY() + 1;
         break;

      case Direction::Left:
         if (this->PositionX == 0)
            return;
         newX = this->GetPositionX() - 1;
         newY = this->GetPositionY();
         break;

      case Direction::Right:
         if (this->PositionX == (currentLandmark->GetWidth() - 1))
            return;
         newX = this->GetPositionX() + 1;
         newY = this->GetPositionY();
         break;
   }

   if (!this->IsPassable(newX, newY)) {

      auto prop = currentLandmark->GetProp(newX, newY);
      if (prop != nullptr &&
            !Tile::HasSurfaceAttribute(Tile::FromTileType(prop->GetTileType()), SurfaceAttribute::Walkable)) {

         bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
         GameState::Get().AddLogMessageFmt("You are blocked by %s %s.", (startsWithVowel ? "an" : "a"),
                                           prop->GetName().c_str());
         return;
      }

      auto tile = currentLandmark->GetTile(newX, newY);
      bool startsWithVowel = tile.Name.find_first_of("aAeEiIoOuU") == 0;
      GameState::Get().AddLogMessageFmt("You are blocked by %s %s.", (startsWithVowel ? "an" : "a"), tile.Name.c_str());
      return;
   }

   this->WarpPlayer(newX, newY);
}

std::shared_ptr<ITool> Player::GetCurrentTool() {
   return std::shared_ptr<ITool>(this->CurrentTool);
}

std::vector<std::shared_ptr<IProp>> Player::GetInventory() {
   return this->Inventory;
}

void Player::SpawnIntoInventory(std::shared_ptr<IProp> prop) {
   this->Inventory.push_back(std::shared_ptr<IProp>(prop));
   bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
   GameState::Get().AddLogMessageFmt("%s %s has been placed in your inventory.", (startsWithVowel ? "An" : "A"),
                                     prop->GetName().c_str());
}

void Player::PickUpItemFromGround() {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto prop = currentLandmark->GetProp(this->GetPositionX(), this->GetPositionY());

   if (prop == nullptr) {
      GameState::Get().AddLogMessage("There is nothing on the ground to pick up.");
      return;
   }

   if (!prop->Takeable()) {
      GameState::Get().AddLogMessageFmt("You cannot pick the %s up.", prop->GetName().c_str());
      return;
   }

   bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
   GameState::Get().AddLogMessageFmt("You pick up %s %s.", startsWithVowel ? "an" : "a", prop->GetName().c_str());

   currentLandmark->RemoveProp(this->GetPositionX(), this->GetPositionY());
   this->SpawnIntoInventory(prop);
}

void Player::EquipFromInventory(std::shared_ptr<ITool> tool) {
   if (this->GetCurrentTool() != nullptr) {
      Player::Get().UnequipCurrentTool();
   }

   auto prop = std::dynamic_pointer_cast<IProp>(tool);
   this->RemoveFromInventory(prop);
   this->CurrentTool = std::shared_ptr<ITool>(tool);

   bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
   GameState::Get().AddLogMessageFmt("You equip %s %s.", startsWithVowel ? "an" : "a", prop->GetName().c_str());
}

void Player::DropInventoryItemOnGround(std::shared_ptr<IProp> prop) {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   this->RemoveFromInventory(prop);
   currentLandmark->AddProp(this->GetPositionX(), this->GetPositionY(), prop);
}

void Player::RemoveFromInventory(std::shared_ptr<IProp> prop) {
   auto i = 0;
   for (auto invProp : this->Inventory) {
      if (invProp != prop) {
         i++;
         continue;
      }

      this->Inventory.erase(this->Inventory.begin() + i);
      bool startsWithVowel = invProp->GetName().find_first_of("aAeEiIoOuU") == 0;
      GameState::Get().AddLogMessageFmt("%s %s has been removed from your inventory.", (startsWithVowel ? "An" : "A"),
                                        invProp->GetName().c_str());
      break;
   }
}

void Player::UseTool() {
   if (this->CurrentTool == nullptr) {
      GameState::Get().AddLogMessage("You do not currently have a tool equipped!");
      return;
   }
   if (!this->CurrentTool->IsUsable()) {
      return;
   }

   this->CurrentTool->Use();
}

void Player::UnequipCurrentTool() {
   if (this->CurrentTool == nullptr) {
      GameState::Get().AddLogMessage("You do not currently have a tool equipped!");
      return;
   }
   this->SpawnIntoInventory(std::dynamic_pointer_cast<IProp>(this->CurrentTool));
   this->CurrentTool = nullptr;
}

bool Player::IsPassable(int x, int y) {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto tile = currentLandmark->GetTile(x, y);
   if (!Tile::HasSurfaceAttribute(tile, SurfaceAttribute::Walkable)) {
      return false;
   }
   auto prop = currentLandmark->GetProp(x, y);
   if (prop == nullptr) {
      return true;
   }

   return Tile::HasSurfaceAttribute(Tile::FromTileType(prop->GetTileType()), SurfaceAttribute::Walkable);
}

int Player::GetEnergy() {
   return this->Energy;
}

void Player::SetEnergy(int energy) {
   if (energy < 0) {
      this->Energy = 0;
      return;
   }
   this->Energy = energy;
}

void Player::AdjustEnergy(int energyAdjustment) {
   this->SetEnergy(this->GetEnergy() + energyAdjustment);
}

void Player::InteractWith() {
   GameState::Get().PushDialog(InteractionDirectionDialog::Construct());
}

void Player::InteractWith(Direction::Direction direction) {
   auto newX = this->PositionX;
   auto newY = this->PositionY;

   switch(direction) {
      case Direction::Up:
         newY--;
         break;
      case Direction::Down:
         newY++;
         break;
      case Direction::Left:
         newX--;
         break;
      case Direction::Right:
         newX++;
         break;
   }

   if (newX < 0 || newY < 0) {
      return;
   }

   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto prop = currentLandmark->GetProp(newX, newY);

   if (prop == nullptr) {
      GameState::Get().AddLogMessage("There is nothing to interact with in that direction.");
      return;
   }

   auto interactable = std::dynamic_pointer_cast<IInteractable>(prop);
   if (interactable == nullptr) {
      GameState::Get().AddLogMessageFmt("The %s cannot be interacted with.", prop->GetName().c_str());
      return;
   }

   interactable->Interact();
}

void Player::SetIsSleeping(bool sleeping) {
   this->IsSleeping = sleeping;
}

bool Player::GetIsSleeping() {
   return this->IsSleeping;
}
