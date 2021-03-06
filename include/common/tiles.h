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
#include "colors.h"

namespace SurfaceAttribute {
   typedef unsigned long SurfaceAttribute;
   const SurfaceAttribute None               = 0x0000;
   const SurfaceAttribute Walkable           = 0x0001;
   const SurfaceAttribute Swimmable          = 0x0002;
   const SurfaceAttribute VisualObstruction  = 0x0004;
   const SurfaceAttribute Invalid            = ~0;

   static SurfaceAttribute FromString(std::string name) 
   {
      if (name == "none") return None;
      if (name == "walkable") return Walkable;
      if (name == "swimmable") return Swimmable;
      if (name == "visualObstruction") return VisualObstruction;
      return Invalid;
   }

   static bool HasAttribute(SurfaceAttribute surfaceAttributes, SurfaceAttribute surfaceAttribute)
   {
      return (surfaceAttributes & surfaceAttribute) > 0;
   }
}


namespace TileType {
   enum TileType {
      Nothing,
      Player,

      Tilled,

      DirtPath,
      Grass,
      GrassTuft,
      //Weed,

      Stone,
      //Boulder,
      //Stump,

      Water,

      BrickWall,
      Door,

      //// Items
      //SeedBag,

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

      //_MAX
   };

}

namespace Tile {
   typedef struct {
      TileType::TileType TileType;
      std::string Name;
      SurfaceAttribute::SurfaceAttribute SurfaceAttributes;
      Color::Color ColorCode;
      char CharacterCode;
      int GfxTileCode;
   } Tile;


   static Tile Tiles[] = {
         {TileType::Nothing,               "Nothing",             SurfaceAttribute::None,              Color::Red,         'X', -1},
         {TileType::Player,                "You",                 SurfaceAttribute::None,              Color::White,       '@',  1},
         {TileType::Tilled,                "Tilled Land",         SurfaceAttribute::Walkable,          Color::Yellow,      '=',  2},
         {TileType::DirtPath,              "Dirt Path",           SurfaceAttribute::Walkable,          Color::White,       '_', 41},
         {TileType::Grass,                 "Grass",               SurfaceAttribute::Walkable,          Color::Green,       '.',  3},
         {TileType::GrassTuft,             "Grass Tuft",          SurfaceAttribute::Walkable,          Color::BrightGreen, ',',  5},
         {TileType::Stone,                 "Stone",               SurfaceAttribute::Walkable,          Color::Gray,        'o', 12},
         {TileType::Water,                 "Water",               SurfaceAttribute::Swimmable,         Color::BrightBlue,  '~', 10},
         {TileType::BrickWall,             "Brick Wall",          SurfaceAttribute::VisualObstruction, Color::BrightRed,   '|', 11},
         {TileType::Door,                  "Wooden Door",         SurfaceAttribute::VisualObstruction |                
                                                                  SurfaceAttribute::Walkable,          Color::Yellow,  '-', 14},
                                                                                                                       
         // UI                                                                                                         
         {TileType::WindowTopLeft,         "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 16},
         {TileType::WindowTop,             "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 17},
         {TileType::WindowTopRight,        "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 18},
         {TileType::WindowLeft,            "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 24},
         {TileType::WindowCenter,          "",                    SurfaceAttribute::None,              Color::White,   ' ', 25},
         {TileType::WindowRight,           "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 26},
         {TileType::WindowBottomLeft,      "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 32},
         {TileType::WindowBottom,          "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 33},
         {TileType::WindowBottomRight,     "",                    SurfaceAttribute::None,              Color::Yellow,  '=', 34}



   };

   static Tile FromTileType(TileType::TileType tileType) {
      return Tiles[tileType];
   }

   static bool HasSurfaceAttribute(Tile tile, SurfaceAttribute::SurfaceAttribute surfaceAttributeType) {
      return ((tile.SurfaceAttributes & surfaceAttributeType) > 0);
   }
}




#endif //HARVEST_ROGUE_TILES_H
