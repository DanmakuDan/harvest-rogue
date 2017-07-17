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
#include "perlinnoise.h"
#include <random>


static std::random_device randomDevice;
static std::default_random_engine randomGenerator(randomDevice());

std::shared_ptr<Landmark> LandmarkGenerator::GeneratePlayerFarm(int &playerX, int &playerY) {
   std::uniform_int_distribution<int> twigDistrobution(0, 1000);

   auto farmName = TextGenerator::GenerateFarmName();
   auto result = Landmark::Construct(farmName, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT);

   siv::PerlinNoise perlinA;
   siv::PerlinNoise perlinB;
   siv::PerlinNoise perlinC;


   // Generate some random stuff on the ground
   for (auto y = 0; y < MAP_SIZE_WIDTH; y++) {
      for (auto x = 0; x < MAP_SIZE_HEIGHT; x++) {
         if ((x < 2) || (x >= (MAP_SIZE_WIDTH - 2)) || (y < 2) || (y >= (MAP_SIZE_HEIGHT - 2))) {
            result->SetTile(x, y, TileType::Water);
            continue;
         }

         auto pa = perlinA.noise0_1(static_cast<double>(x) * 0.1, static_cast<double>(y) * 0.1);
         auto pb = perlinA.noise0_1(static_cast<double>(x+1000) * 0.05, static_cast<double>(y+1000) * 0.05);
         auto pc = perlinA.noise0_1(static_cast<double>(x+90) * 0.002, static_cast<double>(y) * 0.002);
         auto n = twigDistrobution(randomGenerator);

         if (pa > 0.50) {
            if (pb > 0.55) {
               result->SetTile(x, y, TileType::Grass);
               result->AddItem(x, y, GameState::Get().GetItemFromItemDatabase("Birch Tree"));
            } else {
               result->SetTile(x, y, TileType::GrassTuft);
            }
            
         } else if (n < 2) {
            result->SetTile(x, y, TileType::Grass);
            result->AddItem(x, y, GameState::Get().GetItemFromItemDatabase("Birch Twig"));
         }
         else {
            if (pc > 0.75) {
               result->SetTile(x, y, TileType::Water);
            } else {
               result->SetTile(x, y, TileType::Grass);
            }            
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
            result->SetTile(cottageX + x, cottageY + y, TileType::BrickWall);
         } else {
            result->SetTile(cottageX + x, cottageY + y, TileType::Stone);
         }

         auto prop = result->GetItem(cottageX + x, cottageY + y);
         if (prop != nullptr) {
            result->RemoveItem(cottageX + x, cottageY + y);
         }
      }

      // Generate the paths
      auto pathLeft = cottageX - 30;
      if (pathLeft < 5) {
         pathLeft = 5;
      }
      auto pathRight = cottageX + LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH + 30;
      if (pathRight > MAP_SIZE_WIDTH - 5) {
         pathRight = MAP_SIZE_WIDTH - 5;
      }

      result->SetTile(cottageX + (LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH / 2), cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT, TileType::DirtPath);
      {
         auto prop = result->GetItem(cottageX + (LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH / 2), cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT);
         if (prop != nullptr) {
            result->RemoveItem(cottageX + (LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH / 2), cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT);
         }
      }

      for (auto x = pathLeft; x < pathRight; x++) {
         result->SetTile(x, cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT + 1, TileType::DirtPath);
         auto prop = result->GetItem(x, cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT + 1);
         if (prop != nullptr) {
            result->RemoveItem(x, cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT + 1);
         }
      }
   }

   // Place a door
   result->SetTile(cottageX + (LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH / 2),
                   cottageY + LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT - 1, TileType::Stone);

   // Set the player starting position variables
   playerX = cottageX + (LANDMARKGENERATOR_DEFAULT_COTTAGE_WIDTH / 2);
   playerY = cottageY + (LANDMARKGENERATOR_DEFAULT_COTTAGE_HEIGHT / 2);

   // Add the bed
   result->AddItem(cottageX + 1, cottageY + 1, GameState::Get().GetItemFromItemDatabase("Simple Bed"));

   // Add a chest
   result->AddItem(cottageX + 9, cottageY+1, GameState::Get().GetItemFromItemDatabase("Simple Chest"));

   //// Add tools to the ground...
   result->AddItem(playerX - 3, playerY - 1, GameState::Get().GetItemFromItemDatabase("Simple Hoe"));
   result->AddItem(playerX - 2, playerY - 1, GameState::Get().GetItemFromItemDatabase("Simple Axe"));

   //// And some seeds
   auto cabbageSeeds = GameState::Get().GetItemFromItemDatabase("Cabbage Seed");
   cabbageSeeds->SetCount(10);
   result->AddItem(playerX - 2, playerY + 1, cabbageSeeds);

   return result;
}