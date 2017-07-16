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

/// @brief The maximum amount of energy the player can have.
#define ENERGY_MAX         100 

/// @brief The amount of energy the player can have to be well rested.
#define ENERGY_WELL_RESTED  80

/// @brief The amount of energy the player can have to be 'good' on energy.
#define ENERGY_GOOD         50

/// @brief The amount of energy the player can have to be tired.
#define ENERGY_TIRED        30

/// @brief The amount of energy the player can have to be exhausted.
#define ENERGY_EXHAUSTED    15


/*! Represents the player */
class Player: 
   public IItemContainer {
   Player();
   Player(Player const &) = delete;
   Player &operator=(Player const &) = delete;

public:
   ~Player();

   /// Gets the singleton instance of the player.
   static Player &Get() {
      static Player player;
      return player;
   }

   /// Resets the player to the initial starting state.
   void Reset();

   /// Gets the player's X tile position.
   int GetPositionX() const;

   /// Gets the player's Y tile position.
   int GetPositionY() const;

   /// Moves the player to a new position.
   /// @param [in] x The x tile position.
   /// @param [in] y The y tile position.
   void WarpPlayer(int x, int y);

   /// Moves the player one tile in the specified direction.
   /// @param [i] direction The direction to move the player.
   void WalkPlayer(Direction::Direction direction);

   /// Returns a pointer to the currently equipped item.
   ItemPtr GetCurrentlyEquippedItem() const;
   
   /// Gets the item on the tile the player is on, and puts it in the player's inventory.
   void PickUpItemFromGround();
   
   /// Equips the specified item in the player's inventory, and equips it. This method will remove the item from the inventory
   /// @param tool [in] The tool (from the player's inventory) to equip.
   void EquipFromInventory(ItemPtr tool);
   
   /// Drops the specified inventory item from the player's inventory onto the ground. For stacks, this will drop the entire stack
   /// @param prop [in] The inventory item to drop onto the ground.
   void DropInventoryItemOnGround(ItemPtr prop);
   
   /// Executes the Use() method from the item's IUsable interface. If the item is not an IUsable, or if it implements IDurable and 
   /// is broken, the item will not be used and an appropriate message will display in the log.
   void UseEquippedItem() const;
   
   /// Unequips the currently equipped tool, and places it back into the player's inventory. If no item is equipped, nothing will
   /// happen and an appropriate message will display in the log.
   void UnequipCurrentEquippedItem();
   
   /// Returns the amount of energy the player has, between 0 and #ENERGY_MAX.
   int GetEnergy() const;
   
   /// Sets the amount of energy the player has. If it is not between 0 and #ENERGY_MAX, it will be clamped to those values.
   void SetEnergy(int energy);
   
   /// Adjusts the amount of energy the player has. If the resulting energy amount is not between 0 and #ENERGY_MAX, it will be clamped to those values.
   void AdjustEnergy(int energyAdjustment);
  
   /// Initiates the 'Interact With' dialog.
   static void InteractWith();
   
   /// Causes the player to interact in the specified direction.
   /// @param direction [in] The direction in which to interact.
   void InteractWith(Direction::Direction direction) const;
   
   /// Sets the sleeping state of the user.
   /// @param sleeping [in] true if the player is sleeping, false otherwise.
   void SetIsSleeping(bool sleeping);
   
   /// Returns the player's sleep state; true if sleeping, false if awake.
   bool GetIsSleeping() const;
   
   /// Returns the total amount of an item the player is holding across all stacks.
   /// @param ItemName [in] The name of the item to count.
   int GetAmountOfThisItemHeld(std::string ItemName);

   /// Initiates the 'Craft' dialog.
   static void Craft();

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
   /// Transfers an item from the source into the player's inventory.
   /// @param sourceItem [in] The item to transfer. Note that this item MUST be the actual item to transfer, not a definition
   ///        as it removes the item's stack count.
   /// @param amountToTransfer [in] the stack amount to transfer.
   void TransferIntoInventory(ItemPtr sourceItem, int amountToTransfer = MOVE_AMOUNT_EVERYTHING);

   /// Removes an item from the inventory.
   /// @param sourceItem [in] The item in the player's inventory to transfer. This MUST point to an instance of an item in the player's inventory.
   /// @param amountToMove [in] the stack amount to transfer.
   /// @returns A new item containing the item (or item stack) that was removed from the player's inventory.
   ItemPtr RemoveFromInventory(ItemPtr sourceItem, int amountToMove = MOVE_AMOUNT_EVERYTHING);

   /// The X tile position of the player.
   int PositionX;

   /// The Y tile position of the player.
   int PositionY;

   /// The currently equipped item.
   ItemPtr CurrentlyEquippedItem;

   /// The player's inventory.
   ItemListPtr Inventory;

   /// Helper function to determine if the specific x/y location is passible (can be walked onto) by the player.
   static bool IsPassable(int x, int y);

   /// Determines if the player is sleeping or not. If sleeping then this is true; for awake, this value is false.
   bool IsSleeping;

   /// The level of energy the player has, between 0 and #MAX_ENERGY.
   int Energy;
};

#endif //HARVEST_ROGUE_PLAYER_H

