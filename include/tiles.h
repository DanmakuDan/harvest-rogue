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

#ifndef HARVEST_ROGUE_TILES_H
#define HARVEST_ROGUE_TILES_H

#include <string>
#include <vector>
#include "colors.h"

namespace SurfaceAttribute {
   typedef unsigned long SurfaceAttribute;
   const SurfaceAttribute None               = 0x0001;
   const SurfaceAttribute Walkable           = 0x0002;
   const SurfaceAttribute Swimmable          = 0x0004;
   const SurfaceAttribute VisualObstruction  = 0x0008;
}


namespace TileType {
   enum TileType {
      Nothing,
      Player,

      Tilled,

      Grass,
      GrassTuft,
      Weed,

      Stone,
      Branch,
      Boulder,
      Stump,
      Tree,

      Water,

      BrickWall,
      Door,

      Hoe,

      _MAX
   };

}

namespace Tile {
   typedef struct {
      TileType::TileType TileType;
      std::string Name;
      SurfaceAttribute::SurfaceAttribute SurfaceAttributes;
      int ColorCode;
      char CharacterCode;
   } Tile;


   static std::vector<Tile> Tiles = {
         {TileType::Nothing,   "Nothing",     SurfaceAttribute::None,              CLR_RED,     'X'},
         {TileType::Player,    "You",         SurfaceAttribute::None,              CLR_WHITE,   '@'},

         {TileType::Tilled,    "Tilled Land", SurfaceAttribute::Walkable,          CLR_YELLOW,  '='},

         {TileType::Grass,     "Grass",       SurfaceAttribute::Walkable,          CLR_GREEN,   '.'},
         {TileType::GrassTuft, "Grass Tuft",  SurfaceAttribute::Walkable,          CLR_BRGREEN, ','},
         {TileType::Weed,      "Weeds",       SurfaceAttribute::Walkable,          CLR_GREEN,   '"'},

         {TileType::Stone,     "Stone",       SurfaceAttribute::Walkable,          CLR_GRAY,    'o'},
         {TileType::Branch,    "Branch",      SurfaceAttribute::Walkable,          CLR_YELLOW,  '-'},
         {TileType::Boulder,   "Boulder",     SurfaceAttribute::None,              CLR_WHITE,   'O'},
         {TileType::Stump,     "Stump",       SurfaceAttribute::None,              CLR_YELLOW,  '#'},
         {TileType::Tree,      "Tree",        SurfaceAttribute::None,              CLR_YELLOW,  'T'},

         {TileType::Water,     "Water",       SurfaceAttribute::Swimmable,         CLR_BRBLUE,  '~'},

         {TileType::BrickWall, "Brick Wall",  SurfaceAttribute::VisualObstruction, CLR_BRRED,   '|'},

         {TileType::Door,      "Wooden Door", SurfaceAttribute::VisualObstruction |
                                              SurfaceAttribute::Walkable,          CLR_YELLOW,  '-'},
         {TileType::Hoe,       "Hoe",         SurfaceAttribute::Walkable,          CLR_YELLOW,  '\\'}

   };

   static Tile FromTileType(TileType::TileType tileType) {
      for (auto tile : Tiles) {
         if (tile.TileType == tileType) {
            return tile;
         }
      }
      return Tiles.front();
   }

   static bool HasSurfaceAttribute(Tile tile, SurfaceAttribute::SurfaceAttribute surfaceAttributeType) {
      return ((tile.SurfaceAttributes & surfaceAttributeType) > 0);
   }
}




#endif //HARVEST_ROGUE_TILES_H
