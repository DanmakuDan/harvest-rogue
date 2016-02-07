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

#include "landmarkgenerator.h"
#include "gamestate.h"
#include "textgenerator.h"
#include "hoe.h"
#include <random>


static std::random_device randomDevice;
static std::default_random_engine randomGenerator(randomDevice());

std::shared_ptr<Landmark> LandmarkGenerator::GeneratePlayerFarm(int &playerX, int &playerY) {
   std::uniform_int_distribution<int> grassTuftDistrobution(0, 10000);

   auto farmName = TextGenerator::GenerateFarmName();
   auto result = Landmark::Construct(farmName, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT);

   // Generate some random stuff on the ground
   for (auto y = 0; y < MAP_SIZE_WIDTH; y++) {
      for (auto x = 0; x < MAP_SIZE_HEIGHT; x++) {
         if ((x < 2) || (x >= (MAP_SIZE_WIDTH - 2)) || (y < 2) || (y >= (MAP_SIZE_HEIGHT - 2))) {
            result->SetTile(x, y, TileWater);
            continue;
         }

         int n = grassTuftDistrobution(randomGenerator);
         int permille_chance;
         if (n <= (permille_chance = 2500)) {
            result->SetTile(x, y, TileGrassTuft);
         } else if (n <= (permille_chance += 100)) {
            result->SetTile(x, y, TileWeed);
         } else if (n <= (permille_chance += 100)) {
            result->SetTile(x, y, TileBranch);
            //} else if (n <= (permille_chance += 10)) {
            //   result->SetTile(x, y, TileStone);
         } else if (n <= (permille_chance += 10)) {
            result->SetTile(x, y, TileBoulder);
         } else if (n <= (permille_chance += 5)) {
            result->SetTile(x, y, TileStump);
         } else if (n <= (permille_chance += 5)) {
            result->SetTile(x, y, TileTree);
         } else {
            result->SetTile(x, y, TileGrass);
         }

      }
   }

   // Generate cottage
   std::uniform_int_distribution<int> farmPositionDistrobution(10, MAP_SIZE_HEIGHT - 20);
   auto cottageX = farmPositionDistrobution(randomGenerator);
   auto cottageY = farmPositionDistrobution(randomGenerator);

   // Generate the walls and floors
   for (auto y = 0; y < LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT; y++) {
      for (auto x = 0; x < LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH; x++) {
         if ((x == 0) || (x == LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH - 1) || (y == 0) ||
             (y == LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT - 1)) {
            result->SetTile(cottageX + x, cottageY + y, TileBrickWall);
         } else {
            result->SetTile(cottageX + x, cottageY + y, TileStone);
         }

      }
   }

   // Place a door
   result->SetTile(cottageX + (LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH / 2),
                   cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT - 1, TileDoor);

   // Set the player starting position variables
   playerX = cottageX + (LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH / 2);
   playerY = cottageY + (LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT / 2);

   // Add a hoe to the ground...
   result->AddProp(playerX - 3, playerY - 1, Hoe::Construct());

   return result;
}