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
#include "obtainable.h"
#include "useable.h"

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

      auto prop = currentLandmark->GetItem(newX, newY);
      if (prop != nullptr && !SurfaceAttribute::HasAttribute(prop->GetSurfaceAttributes(), SurfaceAttribute::Walkable)) {

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

std::shared_ptr<Item> Player::GetCurrentlyEquippedItem() {
   return std::shared_ptr<Item>(this->CurrentlyEquippedItem);
}

std::vector<std::shared_ptr<PlayerInventoryItem>> Player::GetInventory() {
   return this->Inventory;
}

void Player::SpawnIntoInventory(std::shared_ptr<Item> prop) {

   if (!prop->HasInterface(ItemInterfaceType::Obtainable)) {
      throw;
   }

   auto obtainableInterface = prop->GetInterface<Obtainable>(ItemInterfaceType::Obtainable);
   if (!obtainableInterface->GetIsStackable()) {
      if (prop->GetCount() > 1) {
         throw;
      }

      auto item = std::shared_ptr<PlayerInventoryItem>(new PlayerInventoryItem());
      item->ItemTarget = std::shared_ptr<Item>(prop);
      item->StackSize = 1;
      this->Inventory.push_back(item);

      bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
      if (prop->GetCount() > 0) {
         GameState::Get().AddLogMessageFmt("%s %s has been placed in your inventory.", (startsWithVowel ? "An" : "A"), prop->GetName().c_str());
      }

      return;
   }

   // Item is stackable
   std::shared_ptr<PlayerInventoryItem> destItem = nullptr;
   for (auto item : this->Inventory) {
      if (item->ItemTarget->GetName() != prop->GetName()) {
         continue;
      }

      if (item->StackSize + prop->GetCount() > obtainableInterface->GetMaxStackSize()) {
         auto diff = obtainableInterface->GetMaxStackSize() - item->StackSize;
         item->StackSize = obtainableInterface->GetMaxStackSize();
         prop->SetCount(prop->GetCount() - diff);

         continue;
      }
      
      destItem = std::shared_ptr<PlayerInventoryItem>(item);
      break;
   }

   if (destItem == nullptr) {
      destItem = std::shared_ptr<PlayerInventoryItem>(new PlayerInventoryItem());
      destItem->ItemTarget = std::shared_ptr<Item>(prop);
      destItem->StackSize = 0;
      this->Inventory.push_back(destItem);
   }

   destItem->StackSize += prop->GetCount();
   bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
   if (prop->GetCount() > 0) {
      GameState::Get().AddLogMessageFmt("%s %s has been placed in your inventory.", (startsWithVowel ? "An" : "A"), prop->GetName().c_str());
   }
}

void Player::PickUpItemFromGround() {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto prop = currentLandmark->GetItem(this->GetPositionX(), this->GetPositionY());

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

   currentLandmark->RemoveItem(this->GetPositionX(), this->GetPositionY());
   this->SpawnIntoInventory(prop);
}

void Player::EquipFromInventory(std::shared_ptr<Item> item) {
   if (!item->IsEquippable()) {
      GameState::Get().AddLogMessageFmt("You can't equip the %s.", item->GetName().c_str());
      return;
   }

   if (this->GetCurrentlyEquippedItem() != nullptr) {
      Player::Get().UnequipCurrentEquippedItem();
   }

   this->RemoveFromInventory(item);

   auto newItem = std::make_shared<Item>(Item::Clone(*item.get()));
   newItem->SetCount(1);

   this->CurrentlyEquippedItem = std::shared_ptr<Item>(newItem);
   item->NotifyItemEquipped();
   GameState::Get().AddLogMessageFmt("You equip the %s.", newItem->GetName().c_str());
}

void Player::DropInventoryItemOnGround(std::shared_ptr<Item> prop) {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   if (currentLandmark->GetItem(this->GetPositionX(), this->GetPositionY()) != nullptr) {
      bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
      GameState::Get().AddLogMessageFmt("Cannot drop %s %s, Something is already on the ground here.", startsWithVowel ? "an" : "a", prop->GetName().c_str());
      return;
   }
   this->RemoveFromInventory(prop);


   auto newProp = std::make_shared<Item>(Item::Clone(*prop.get()));
   newProp->SetCount(1);
   currentLandmark->AddItem(this->GetPositionX(), this->GetPositionY(), newProp);
}

bool Player::RemoveFromInventory(std::shared_ptr<Item> prop) {
   auto i = 0;
   for (auto invProp : this->Inventory) {
      if (invProp->ItemTarget != prop) {
         i++;
         continue;
      }

      if (--this->Inventory[i]->StackSize <= 0) {
         this->Inventory.erase(this->Inventory.begin() + i);
      }

      bool startsWithVowel = invProp->ItemTarget->GetName().find_first_of("aAeEiIoOuU") == 0;
      GameState::Get().AddLogMessageFmt("%s %s has been removed from your inventory.", (startsWithVowel ? "An" : "A"),
                                        invProp->ItemTarget->GetName().c_str());
      
      return true;
   }

   return false;
}

void Player::UseEquippedItem() {
   if (this->CurrentlyEquippedItem == nullptr) {
      GameState::Get().AddLogMessage("You do not currently have an item equipped!");
      return;
   }
   if (!this->CurrentlyEquippedItem->IsUsable()) {
      return;
   }

   this->CurrentlyEquippedItem->Use();
}

void Player::UnequipCurrentEquippedItem() {
   if (this->CurrentlyEquippedItem == nullptr) {
      GameState::Get().AddLogMessage("You do not currently have a tool equipped!");
      return;
   }
   this->SpawnIntoInventory(std::dynamic_pointer_cast<Item>(this->CurrentlyEquippedItem));
   this->CurrentlyEquippedItem->NotifyItemUnequiupped();
   this->CurrentlyEquippedItem = nullptr;
}

bool Player::IsPassable(int x, int y) {
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto tile = currentLandmark->GetTile(x, y);
   if (!Tile::HasSurfaceAttribute(tile, SurfaceAttribute::Walkable)) {
      return false;
   }
   auto prop = currentLandmark->GetItem(x, y);
   if (prop == nullptr) {
      return true;
   }

   return SurfaceAttribute::HasAttribute(prop->GetSurfaceAttributes(), SurfaceAttribute::Walkable);
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
   auto prop = currentLandmark->GetItem(newX, newY);

   if (prop == nullptr) {
      GameState::Get().AddLogMessage("There is nothing to interact with in that direction.");
      return;
   }

   /*auto interactable = std::dynamic_pointer_cast<IInteractable>(prop);
   if (interactable == nullptr) {
      GameState::Get().AddLogMessageFmt("The %s cannot be interacted with.", prop->GetName().c_str());
      return;
   }

   interactable->Interact();
   */
}

void Player::SetIsSleeping(bool sleeping) {
   this->IsSleeping = sleeping;
}

bool Player::GetIsSleeping() {
   return this->IsSleeping;
}
