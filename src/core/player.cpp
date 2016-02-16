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
   Reset();
}

Player::~Player() {

}

void Player::Reset()
{
   this->Energy = ENERGY_MAX;
   this->IsSleeping = false;
   this->Inventory.clear();
   this->CurrentlyEquippedItem = nullptr;
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

ItemPtr Player::GetCurrentlyEquippedItem() {
   return ItemPtr(this->CurrentlyEquippedItem);
}

ItemListPtr Player::GetInventory() {
   return this->Inventory;
}

void Player::TransferIntoInventory(ItemPtr sourceItem, int amountToTransfer) {
   
   // The player cannot obtain items that aren't obtainable
   if (!sourceItem->HasInterface(ItemInterfaceType::Obtainable)) {
      throw;
   }

   auto obtainableInterface = sourceItem->GetInterface<Obtainable>(ItemInterfaceType::Obtainable);

   // A MOVE_AMOUNT_EVERYTHING of amountToTransfer (the default value) means we want to transfer all of the items
   if (amountToTransfer == MOVE_AMOUNT_EVERYTHING) {
      amountToTransfer = sourceItem->GetCount();
   }

   // If there are no items in the stack, or the transfer amount is nothing, then there is nothing to transfer.
   if (sourceItem->GetCount() <= 0 || amountToTransfer == 0) {
      return;
   }

   // Do a sanity check and make sure we are not trying to transfer more items than exists in the stack
   if (sourceItem->GetCount() < amountToTransfer) {
      throw;
   }

   // First determine if the item is stackable. 
   if (!obtainableInterface->GetIsStackable()) {

      // The item is not stackable, so we know we need to create it in the inventory.
      auto destItem = std::make_shared<Item>(Item::Clone(*sourceItem));

      // Do a sanity check to make sure there's at most 1 item present.
      if (sourceItem->GetCount() > 1) {
         throw;
      }

      // Move the counts from the source to destination items
      destItem->SetCount(amountToTransfer);
      sourceItem->SetCount(sourceItem->GetCount() - amountToTransfer);

      // Add the new item to the inventory
      this->Inventory.push_back(destItem);

      // Destroy the source item if there's none left
      if (sourceItem->GetCount() <= 0) {
         sourceItem->Destruct(false);
      }

      // Notify the player, then return
      GameState::Get().AddLogMessageFmt("%i %s has been placed in your inventory.", amountToTransfer, sourceItem->GetName().c_str());
      return;
   }

   // If we made it here, the item is stackable
   // Loop through the inventory and see if we can place the items into an existing stack.

   for (auto destItem : this->Inventory) {

      // Loop until we find an item in our inventory that matches the one we're spawning
      if (destItem->GetName() != sourceItem->GetName()) {
         continue;
      }

      // If we got here, we've found an item in the inventory matching the one we're spawning.
      // If the item stack is already full, move on to the next item in the inventory
      if (destItem->GetCount() == obtainableInterface->GetMaxStackSize()) {
         continue;
      }

      // See if this stack will be able to contain the entire stack of items we're spawning
      if (destItem->GetCount() + sourceItem->GetCount() > obtainableInterface->GetMaxStackSize()) {

         // This item's stack size isn't enough to contain all the items we're spawning, so
         // max out this stack, and subtract it out of the items left to spawn
         auto diff = obtainableInterface->GetMaxStackSize() - destItem->GetCount();
         destItem->SetCount(obtainableInterface->GetMaxStackSize());
         sourceItem->SetCount(sourceItem->GetCount() - diff);

         // Loop back to the next inventory item, maybe there's more items of the same type in the stack
         continue;
      }
      
      // We've found an inventory item with enough capacity to take the rest of the items, so transfer
      // the stack to the inventory item
      destItem->SetCount(destItem->GetCount() + sourceItem->GetCount());
      
      // And destroy the original item
      sourceItem->Destruct(false);

      GameState::Get().AddLogMessageFmt("%i %s has been placed in your inventory.", amountToTransfer, destItem->GetName().c_str());
      return;
   }

   // We haven't found an item stack for this item yet, so let's simply move the item into
   // the inventory

   auto newItem = std::make_shared<Item>(Item::Clone(*sourceItem.get()));
   newItem->SetCount(sourceItem->GetCount());
   this->Inventory.push_back(newItem);
   sourceItem->Destruct(false);


   GameState::Get().AddLogMessageFmt("%i %s has been placed in your inventory.", amountToTransfer, newItem->GetName().c_str());
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

   this->TransferIntoInventory(prop, MOVE_AMOUNT_EVERYTHING);
}

void Player::EquipFromInventory(ItemPtr item) {
   if (!item->IsEquippable()) {
      GameState::Get().AddLogMessageFmt("You can't equip the %s.", item->GetName().c_str());
      return;
   }

   if (this->GetCurrentlyEquippedItem() != nullptr) {
      Player::Get().UnequipCurrentEquippedItem();
   }

   auto newItem = this->RemoveFromInventory(item, MOVE_AMOUNT_EVERYTHING);
   this->CurrentlyEquippedItem = ItemPtr(newItem);
   item->NotifyItemEquipped();
   GameState::Get().AddLogMessageFmt("You equip the %s.", newItem->GetName().c_str());
}

void Player::DropInventoryItemOnGround(ItemPtr prop) {

   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   if (currentLandmark->GetItem(this->GetPositionX(), this->GetPositionY()) != nullptr) {

      bool startsWithVowel = prop->GetName().find_first_of("aAeEiIoOuU") == 0;
      GameState::Get().AddLogMessageFmt("Cannot drop %s %s, Something is already on the ground here.", startsWithVowel ? "an" : "a", prop->GetName().c_str());

      return;
   }

   auto newProp = this->RemoveFromInventory(prop, 1);

   currentLandmark->AddItem(this->GetPositionX(), this->GetPositionY(), newProp);
}

ItemPtr Player::RemoveFromInventory(ItemPtr sourceItem, int amountToMove) {
   auto i = 0;

   auto result = std::make_shared<Item>(Item::Clone(*sourceItem.get()));
   
   // Loop through the inventory until we find our item
   for (auto invProp : this->Inventory) {

      // If not, move on to the next item
      if (invProp != sourceItem) {

         // (We're also keeping track of the index we're at)
         i++;
         continue;
      }

      // Are we moving the entire stack?
      if (amountToMove == MOVE_AMOUNT_EVERYTHING || amountToMove == invProp->GetCount()) {

         // Transfer all the things
         result->SetCount(invProp->GetCount());

         // And remove the source item
         this->Inventory.erase(this->Inventory.begin() + i);
         invProp->Destruct(false);

         // All done!
         return result;
      }

      
      // Otherwise, we're going to remove ~some~ of the items
      result->SetCount(amountToMove);
      invProp->SetCount(invProp->GetCount() - amountToMove);


      GameState::Get().AddLogMessageFmt("%i %s has been removed from your inventory.", amountToMove, invProp->GetName().c_str());
      
      return result;
   }

   return nullptr;
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

   // Note: When the item is transferred into inventory, there is a check to
   //       ensure it is not currently equipped. You *MUST* clear the equipped
   //       item *before* transferring it or you will get the callstack of death.
   ItemPtr currentItem = this->CurrentlyEquippedItem;
   this->CurrentlyEquippedItem = nullptr;

   this->TransferIntoInventory(currentItem);
   this->CurrentlyEquippedItem->NotifyItemUnequiupped();
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

   if (!prop->IsInteractable()) {
      GameState::Get().AddLogMessageFmt("The %s cannot be interacted with.", prop->GetName().c_str());
      return;
   }

   prop->Interact();
}

void Player::SetIsSleeping(bool sleeping) {
   this->IsSleeping = sleeping;
}

bool Player::GetIsSleeping() {
   return this->IsSleeping;
}
