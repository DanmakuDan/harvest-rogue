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
#include <memory>
#include <vector>

#define ENERGY_MAX         100
#define ENERGY_WELL_RESTED  80
#define ENERGY_GOOD         50
#define ENERGY_TIRED        30
#define ENERGY_EXAUSTED     15

struct PlayerInventoryItem {
   std::shared_ptr<Item> Item;
   int StackSize;
};

class Player {
private:
   Player();
   Player(Player const &) { };
   Player &operator=(Player const &) { };
public:
   ~Player();
   static Player &Get() {
      static Player player;
      return player;
   }

   int GetPositionX();
   int GetPositionY();
   void WarpPlayer(int x, int y);
   void WalkPlayer(Direction::Direction direction);
   std::shared_ptr<Item> GetCurrentTool();
   std::vector<std::shared_ptr<PlayerInventoryItem>> GetInventory();
   void SpawnIntoInventory(std::shared_ptr<Item> prop);
   void RemoveFromInventory(std::shared_ptr<Item> prop);
   void PickUpItemFromGround();
   void EquipFromInventory(std::shared_ptr<Item> tool);
   void DropInventoryItemOnGround(std::shared_ptr<Item> prop);
   void UseTool();
   void UnequipCurrentTool();
   int GetEnergy();
   void SetEnergy(int energy);
   void AdjustEnergy(int energyAdjustment);
   void InteractWith();
   void InteractWith(Direction::Direction direction);
   void SetIsSleeping(bool sleeping);
   bool GetIsSleeping();
private:
   int PositionX;
   int PositionY;
   std::shared_ptr<Item> CurrentTool;
   std::vector<std::shared_ptr<PlayerInventoryItem>> Inventory;
   bool IsPassable(int x, int y);
   bool IsSleeping;
   int Energy;
};

#endif //HARVEST_ROGUE_PLAYER_H

