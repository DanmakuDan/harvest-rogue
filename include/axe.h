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
#ifndef HARVEST_ROGUE_AXE_H
#define HARVEST_ROGUE_AXE_H

#include "prop.h"
#include "tool.h"
#include "tiles.h"
#include "directionaltool.h"

class Axe : public IProp, public ITool, public IDirectionalTool,
      public std::enable_shared_from_this<Axe> {
public:
   static std::shared_ptr<Axe> Construct() {
      return std::shared_ptr<Axe>(new Axe());
   }

   // IProp
   virtual std::string GetName();
   virtual std::string GetDescription();
   virtual TileType::TileType GetTileType();
   virtual bool Takeable();

   // ITool
   virtual bool IsUsable();
   virtual void Use();

   // IDirectionalTool
   virtual void Use(Direction::Direction direction);

private:
   Axe();
};


#endif //HARVEST_ROGUE_AXE_H
