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

#include "item.h"
#include "gamestate.h"
#include "durable.h"
#include "dropsloot.h"
#include "plantable.h"
#include "obtainable.h"
#include "player.h"
#include "useable.h"
#include "equippable.h"
#include "interactable.h"

Item Item::Clone(const Item & source)
{
   Item result = source;

   for (auto i : source.ItemInterfaces) {
      if (i.second == nullptr) {
         result.ItemInterfaces[i.first] = nullptr;
         continue;
      }
      result.ItemInterfaces[i.first] = std::shared_ptr<IItemInterface>(i.second->Clone());
   }

   return result;
}

Item::Item()
{
   this->Count = 1;
}

std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> Item::GetInterfaces()
{
   return this->ItemInterfaces;
}

bool Item::HasInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType)
{
   return this->ItemInterfaces[itemInterfaceType] != nullptr;
}

void Item::AddInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType, std::shared_ptr<IItemInterface> itemInterface)
{
   if (this->ItemInterfaces[itemInterfaceType] != nullptr) {
      throw;
   }

   this->ItemInterfaces[itemInterfaceType] = std::shared_ptr<IItemInterface>(itemInterface);
}

void Item::RemoveInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType)
{
   this->ItemInterfaces.erase(itemInterfaceType);
}

std::string Item::GetName()
{
   return this->Name;
}

void Item::SetName(std::string name)
{
   this->Name = name;
}

std::string Item::GetDescription()
{
   return this->Description;
}

bool Item::IsTakeable()
{
   return this->HasInterface(ItemInterfaceType::Obtainable);
}

bool Item::IsUsable()
{
   std::shared_ptr<IUseable> usableItem;
   
   for (auto i : this->GetInterfaces()) {
      usableItem = std::dynamic_pointer_cast<IUseable>(i.second);
      if (usableItem != nullptr) {
         break;
      }
   }

   if (usableItem == nullptr) {
      return false;
   }

   if (this->HasInterface(ItemInterfaceType::Durable)) {
      auto durableInterface = this->GetInterface<Durable>(ItemInterfaceType::Durable);
      if (durableInterface->GetDurability() <= 0) {
         GameState::Get().AddLogMessageFmt("The %s is broken and cannot be used.", this->GetName().c_str());
         return false;
      }
   }

   return true;
}

bool Item::IsEquippable()
{
   std::shared_ptr<IEquippable> equippableItem;

   for (auto i : this->GetInterfaces()) {
      equippableItem = std::dynamic_pointer_cast<IEquippable>(i.second);
      if (equippableItem != nullptr) {
         break;
      }
   }

   if (equippableItem == nullptr) {
      return false;
   }

   if (this->HasInterface(ItemInterfaceType::Durable)) {
      auto durableInterface = this->GetInterface<Durable>(ItemInterfaceType::Durable);
      if (durableInterface->GetDurability() <= 0) {
         GameState::Get().AddLogMessageFmt("The %s is broken and cannot be equipped.", this->GetName().c_str());
         return false;
      }
   }

   return true;
}

bool Item::IsInteractable()
{
   for (auto i : this->GetInterfaces()) {
      if (std::dynamic_pointer_cast<IInteractable>(i.second) != nullptr) {
         return true;
      }
   }

   return false;
}

void Item::Interact()
{
   std::shared_ptr<IInteractable> interactableItem;

   for (auto i : this->GetInterfaces()) {
      interactableItem = std::dynamic_pointer_cast<IInteractable>(i.second);
      if (interactableItem != nullptr) {
         break;
      }
   }

   if (interactableItem == nullptr) {
      return ;
   }

   interactableItem->Interact(this->shared_from_this());
}

void Item::Use()
{
   std::shared_ptr<IUseable> usableItem;

   for (auto i : this->GetInterfaces()) {
      usableItem = std::dynamic_pointer_cast<IUseable>(i.second);
      if (usableItem != nullptr) {
         break;
      }
   }

   if (usableItem == nullptr) {
      return;
   }

   usableItem->Use(this->shared_from_this());
}

void Item::Use(Direction::Direction direction)
{
   std::shared_ptr<IDirectionallyUsable> usableItem;

   for (auto i : this->GetInterfaces()) {
      usableItem = std::dynamic_pointer_cast<IDirectionallyUsable>(i.second);
      if (usableItem != nullptr) {
         break;
      }
   }

   if (usableItem == nullptr) {
      return;
   }

   usableItem->Use(this->shared_from_this(), direction);
}

void Item::Destruct(bool dropLoot)
{
   int x, y;
   bool itemFoundOnGround = false;

   this->SetCount(0);

   // Make sure this item doesn't exist in a landmark
   for (auto landmark : GameState::Get().GetAllLandmarks()) {
      
      if (landmark->LocateItem(this->shared_from_this(), x, y)) {
         landmark->RemoveItem(x, y);
         itemFoundOnGround = true;
         break;
      }
   }
   
   // If this item is currently equipped on the player, unequip it
   if (Player::Get().GetCurrentlyEquippedItem() == this->shared_from_this()) {
      Player::Get().UnequipCurrentEquippedItem();
   }

   // Remove this item from the player's inventory, if it is there.
   Player::Get().RemoveItem(this->shared_from_this(), MOVE_AMOUNT_EVERYTHING);

   // If this item drops loot, now's the time to drop it, if it was on the ground to begin with
   if (dropLoot && itemFoundOnGround && this->HasInterface(ItemInterfaceType::DropsLoot)) {
      auto dropsLootInterface = this->GetInterface<DropsLoot>(ItemInterfaceType::DropsLoot);
      dropsLootInterface->DropLoot(x, y);
   }
}

void Item::RemoveOne()
{
   if (!this->HasInterface(ItemInterfaceType::Obtainable)) {
      this->Destruct(false);
      return;
   }

   auto obtainable = this->GetInterface<Obtainable>(ItemInterfaceType::Obtainable);
   if (!obtainable->GetIsStackable() || this->Count <= 1) {
      this->Destruct(false);
      return;
   }

   this->Count--;
}

void Item::NotifyItemEquipped()
{
}

void Item::NotifyItemUnequiupped()
{
}

void Item::SetDescription(std::string description)
{
   this->Description = description;
}

SurfaceAttribute::SurfaceAttribute Item::GetSurfaceAttributes()
{
   return this->SurfaceAttributes;
}

void Item::SetSurfaceAttributes(SurfaceAttribute::SurfaceAttribute surfaceAttributes)
{
   this->SurfaceAttributes = surfaceAttributes;
}

void Item::SetColorCode(Color::Color colorCode)
{
   this->ColorCode = colorCode;
}

Color::Color Item::GetColorCode()
{
   return this->ColorCode;
}

void Item::SetCharacterCode(char characterCode)
{
   this->CharacterCode = characterCode;
}

char Item::GetCharacterCode()
{
   return this->CharacterCode;
}

void Item::SetGfxTileCode(int gfxTileCode)
{
   this->GfxTileCode = gfxTileCode;
}

int Item::GetGfxTileCode()
{
   return this->GfxTileCode;
}

std::list<ItemCategory::ItemCategory> Item::GetItemCategories()
{
   return this->ItemCategories;
}

void Item::SetItemCategories(std::list<ItemCategory::ItemCategory> itemCategories)
{
   this->ItemCategories = itemCategories;
}

int Item::GetCount()
{
   return this->Count;
}

void Item::SetCount(int count)
{
   this->Count = count;
}
