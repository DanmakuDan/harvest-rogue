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
#include "screen.h"

enum eSurfaceAttributeType {
   NoSurfaceAttributes = 0x00,
   Walkable = 0x01,
   Swimmable = 0x02,
   VisualObstruction = 0x04
};


enum eTileType {
   TileNothing,
   TilePlayer,
   
   TileTilled,

   TileGrass,
   TileGrassTuft,
   TileWeed,

   TileStone,
   TileBranch,
   TileBoulder,
   TileStump,
   TileTree,

   TileWater,

   TILES_MAX
};

typedef struct tile_s {
   eTileType TileType;
   std::string Name;
   eSurfaceAttributeType SurfaceAttributes;
   int ColorCode;
   char CharacterCode;
} Tile;


static std::vector<Tile> Tiles = {
      {TileNothing,    "Nothing",      NoSurfaceAttributes,  CLR_RED,      'X'},
      {TilePlayer,     "You",          NoSurfaceAttributes,  CLR_WHITE,    '@'},

      {TileTilled,     "Tilled Land",  Walkable,             CLR_YELLOW,   '='},

      {TileGrass,      "Grass",        Walkable,             CLR_GREEN,    '.'},
      {TileGrassTuft,  "Grass Tuft",   Walkable,             CLR_BRGREEN,  ','},
      {TileWeed,       "Weeds",        Walkable,             CLR_GREEN,    '"'},

      {TileStone,      "Stone",        Walkable,             CLR_GRAY,     'o'},
      {TileBranch,     "Branch",       Walkable,             CLR_YELLOW,   '-'},
      {TileBoulder,    "Boulder",      NoSurfaceAttributes,  CLR_WHITE,    'O'},
      {TileStump,      "Stump",        NoSurfaceAttributes,  CLR_YELLOW,   '#'},
      {TileTree,       "Tree",         NoSurfaceAttributes,  CLR_YELLOW,   'T'},

      {TileWater,      "Water",        (Swimmable),          CLR_BRBLUE,   '~'},
};

static Tile FindTilebyTileType(eTileType tileType) {
   for (auto tile : Tiles) {
      if (tile.TileType == tileType) {
         return tile;
      }
   }
   return Tiles.front();
}

static bool TileHasSurfaceAttribute(Tile tile, eSurfaceAttributeType surfaceAttributeType) {
   return ((tile.SurfaceAttributes & surfaceAttributeType) > 0);
}

#endif //HARVEST_ROGUE_TILES_H
