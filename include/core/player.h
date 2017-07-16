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

#ifndef HARVEST_ROGUE_PLAYER_H
#define HARVEST_ROGUE_PLAYER_H

#include "direction.h"
#include "item.h"
#include "nameable.h"
#include "itemcontainer.h"
#include <memory>
#include <vector>

#define ENERGY_MAX         100
#define ENERGY_WELL_RESTED  80
#define ENERGY_GOOD         50
#define ENERGY_TIRED        30
#define ENERGY_EXAUSTED     15

class Player : 
   public IItemContainer {
   Player();
   Player(Player const &) = delete;
   Player &operator=(Player const &) = delete;
public:
   ~Player();
   static Player &Get() {
      static Player player;
      return player;
   }

   void Reset();
   int GetPositionX() const;
   int GetPositionY() const;
   void WarpPlayer(int x, int y);
   void WalkPlayer(Direction::Direction direction);
   ItemPtr GetCurrentlyEquippedItem() const;
   void PickUpItemFromGround();
   void EquipFromInventory(ItemPtr tool);
   void DropInventoryItemOnGround(ItemPtr prop);
   void UseEquippedItem() const;
   void UnequipCurrentEquippedItem();
   int GetEnergy() const;
   void SetEnergy(int energy);
   void AdjustEnergy(int energyAdjustment);
   static void InteractWith();
   void InteractWith(Direction::Direction direction) const;
   void SetIsSleeping(bool sleeping);
   bool GetIsSleeping() const;
   int GetAmountOfThisItemHeld(std::string ItemName);
   void Craft();

   // INameable
   std::string GetName() override;

   // IItemContainer
   ItemListPtr GetAllItems() override;
   ItemContainerPtr AsItemContainer() override;
   void AddItem(ItemPtr item, int count, bool dontStack) override;
   void RemoveItem(ItemPtr item, int count) override;
   void SwapItem(ItemPtr itemA, ItemPtr itemB) override;
   void SplitItem(ItemPtr item) override;
   void CombineItems(ItemPtr source, ItemPtr dest) override;

private:
   void TransferIntoInventory(ItemPtr sourceItem, int amountToTransfer = MOVE_AMOUNT_EVERYTHING);
   ItemPtr RemoveFromInventory(ItemPtr sourceItem, int amountToMove = MOVE_AMOUNT_EVERYTHING);


   int PositionX;
   int PositionY;
   ItemPtr CurrentlyEquippedItem;
   ItemListPtr Inventory;
   static bool IsPassable(int x, int y);
   bool IsSleeping;
   int Energy;
};

#endif //HARVEST_ROGUE_PLAYER_H

