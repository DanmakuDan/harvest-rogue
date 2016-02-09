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

#ifndef HARVEST_ROGUE_TREE_H
#define HARVEST_ROGUE_TREE_H

#include "prop.h"
#include "axe.h"

class Tree : public IProp,
  public std::enable_shared_from_this<Tree> {
public:
   static std::shared_ptr<Tree> Construct() {
      return std::shared_ptr<Tree>(new Tree());
   }

   // IProp
   virtual std::string GetName();
   virtual std::string GetDescription();
   virtual TileType::TileType GetTileType();
   virtual bool Takeable();

private:
   Tree();
   int Durability;
};


#endif //HARVEST_ROGUE_TREE_H
