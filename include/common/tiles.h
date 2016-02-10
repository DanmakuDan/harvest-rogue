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

      // Tools
      Hoe,
      Axe,

      // Items
      SeedBag,

      // UI
      WindowTopLeft,
      WindowTop,
      WindowTopRight,
      WindowLeft,
      WindowCenter,
      WindowRight,
      WindowBottomLeft,
      WindowBottom,
      WindowBottomRight,

      // Crops
      PotatoSeedling,
      PotatoGrowing,
      PotatoFullyGrown,
      PotatoWilted,
      Potato,

      WheatSeedling,
      WheatGrowing,
      WheatFullyGrown,
      WheatWilted,
      Wheat,

      // Interactables
      Bed,

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
      int GfxTileCode;
   } Tile;


   static std::vector<Tile> Tiles = {
         {TileType::Nothing,               "Nothing",             SurfaceAttribute::None,              CLR_RED,     'X', -1},
         {TileType::Player,                "You",                 SurfaceAttribute::None,              CLR_WHITE,   '@',  1},
                                                                                                                   
         {TileType::Tilled,                "Tilled Land",         SurfaceAttribute::Walkable,          CLR_YELLOW,  '=',  2},
                                                                                                                    
         {TileType::Grass,                 "Grass",               SurfaceAttribute::Walkable,          CLR_GREEN,   '.',  3},
         {TileType::GrassTuft,             "Grass Tuft",          SurfaceAttribute::Walkable,          CLR_BRGREEN, ',',  5},
         {TileType::Weed,                  "Weeds",               SurfaceAttribute::Walkable,          CLR_GREEN,   '"',  4},
                                                                                                                       
         {TileType::Stone,                 "Stone",               SurfaceAttribute::Walkable,          CLR_GRAY,    'o', 12},
         {TileType::Branch,                "Branch",              SurfaceAttribute::Walkable,          CLR_YELLOW,  '-',  7},
         {TileType::Boulder,               "Boulder",             SurfaceAttribute::None,              CLR_WHITE,   'O',  6},
         {TileType::Stump,                 "Stump",               SurfaceAttribute::None,              CLR_YELLOW,  '#',  8},
         {TileType::Tree,                  "Tree",                SurfaceAttribute::None,              CLR_YELLOW,  'T',  9},
                                                                                                                      
         {TileType::Water,                 "Water",               SurfaceAttribute::Swimmable,         CLR_BRBLUE,  '~', 10},
                                                                                                                       
         {TileType::BrickWall,             "Brick Wall",          SurfaceAttribute::VisualObstruction, CLR_BRRED,   '|', 11},
                                                                                                                       
         {TileType::Door,                  "Wooden Door",         SurfaceAttribute::VisualObstruction |                
                                                                  SurfaceAttribute::Walkable,          CLR_YELLOW,  '-', 14},
         {TileType::Bed,                   "Bed",                 SurfaceAttribute::None,              CLR_BRBLUE,  'B', 13},
         {TileType::Hoe,                   "Hoe",                 SurfaceAttribute::Walkable,          CLR_YELLOW,  '\\', 15},
         {TileType::Axe,                   "Axe",                 SurfaceAttribute::Walkable,          CLR_WHITE,   'P', 19},
         {TileType::SeedBag,               "Seed bag",            SurfaceAttribute::Walkable,          CLR_WHITE,   'b', 20},
                                                                                                                       
         // UI                                                                                                         
         {TileType::WindowTopLeft,         "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 16},
         {TileType::WindowTop,             "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 17},
         {TileType::WindowTopRight,        "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 18},
         {TileType::WindowLeft,            "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 24},
         {TileType::WindowCenter,          "",                    SurfaceAttribute::None,              CLR_WHITE,   ' ', 25},
         {TileType::WindowRight,           "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 26},
         {TileType::WindowBottomLeft,      "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 32},
         {TileType::WindowBottom,          "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 33},
         {TileType::WindowBottomRight,     "",                    SurfaceAttribute::None,              CLR_YELLOW,  '=', 34},
                                                                                                                      
         // Crops                                                                                                     
         {TileType::PotatoSeedling,        "Potato Seedling",     SurfaceAttribute::Walkable,          CLR_YELLOW,  'x', 35},
         {TileType::PotatoGrowing,         "Growing Potato",      SurfaceAttribute::None,              CLR_GREEN,   'o', 36},
         {TileType::PotatoFullyGrown,      "Fully grown Potato",  SurfaceAttribute::None,              CLR_YELLOW,  'O', 37},
         {TileType::PotatoWilted,          "Rotten Potato",       SurfaceAttribute::None,              CLR_YELLOW,  '~', 21},
         {TileType::Potato,                "Potato",              SurfaceAttribute::None,              CLR_YELLOW,  'O', -1},
                                                                                                                       
         {TileType::WheatSeedling,         "Wheat Seedling",      SurfaceAttribute::Walkable,          CLR_GREEN,   'x', 27},
         {TileType::WheatGrowing,          "Growing Wheat",       SurfaceAttribute::None,              CLR_GREEN,   'X', 28},
         {TileType::WheatFullyGrown,       "Fully grown Wheat",   SurfaceAttribute::None,              CLR_BRYELLOW,'|', 29},
         {TileType::WheatWilted,           "Ruined Wheat",        SurfaceAttribute::None,              CLR_YELLOW,  'f', 21},
         {TileType::Wheat,                 "Wheat",               SurfaceAttribute::None,              CLR_BRYELLOW,'Y', -1}



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
