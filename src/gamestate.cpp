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

#include <random>
#include "gamestate.h"
#include "textgenerator.h"

static std::random_device randomDevice;
static std::default_random_engine randomGenerator(randomDevice());

GameState::GameState() {
   this->active = true;
   this->CurrentScene = nullptr;
   this->NextScene = nullptr;
   this->CurrentLandmarkIndex = -1;
   this->PlayerX = 5;
   this->PlayerY = 5;

   // Temporary debug stuff
   this->AddLogMessage("Test log line 1");
   this->AddLogMessage("Test log line 2");
   this->AddLogMessage("Test log line 3");
   this->AddLogMessage("Test log line 4");
   this->AddLogMessage("Test log line 5");
   this->AddLogMessage("Test log line 6");
   this->AddLogMessage("Test log line 7");

}

GameState::~GameState() {

}

std::shared_ptr<IScene> GameState::GetCurrentScene() {
   return std::shared_ptr<IScene>(this->CurrentScene);
}

void GameState::SetCurrentScene(std::shared_ptr<IScene> newScene) {
   this->NextScene = std::shared_ptr<IScene>(newScene);
}

bool GameState::IsActive() {
   return this->active;
}

void GameState::Terminate() {
   this->active = false;
}


bool GameState::TransitionScene() {
   if (nullptr == this->NextScene) {
      return false;
   }

   this->CurrentScene = std::shared_ptr<IScene>(this->NextScene);
   this->NextScene = nullptr;

   return true;
}

void GameState::InitializeNewGame() {
   CurrentDay = 1;
   CurrentSeason = (eGameStateSeason) 0;
   CurrentYear = 1;
   CurrentHour = 6;
   CurrentMinute = 0;
   CurrentSecond = 0;

   this->Landmarks.clear();
   this->Landmarks.push_back(GeneratePlayerFarm());
   this->CurrentLandmarkIndex = this->Landmarks.size() - 1;
}

void GameState::StepSimulation() {
   CurrentSecond += 20;

   CurrentMinute += CurrentSecond / 60;
   CurrentSecond %= 60;

   CurrentHour += CurrentMinute / 60;
   CurrentMinute %= 60;

   CurrentDay += CurrentHour / 24;
   CurrentHour %= 24;

   while (CurrentDay > 30) {
      CurrentDay -= 30;
      CurrentSeason = (eGameStateSeason) ((int) CurrentSeason + 1);
      if ((int) CurrentSeason > 3) {
         CurrentSeason = (eGameStateSeason) 0;
         CurrentYear++;
      }
      std::stringstream seasonNotice;
      seasonNotice << "It is now " << eGameStateSeasonDescs[CurrentSeason] << "!";
      this->AddLogMessage(seasonNotice.str());
   }
}

int GameState::GetCurrentDay() {
   return this->CurrentDay;
}

eGameStateSeason GameState::GetCurrentSeason() {
   return this->CurrentSeason;
}

int GameState::GetCurrentYear() {
   return this->CurrentYear;
}

int GameState::GetCurrentHour() {
   return this->CurrentHour;
}

int GameState::GetCurrentMinute() {
   return this->CurrentMinute;
}

int GameState::GetCurrentSecond() {
   return this->CurrentSecond;
}

void GameState::AddLogMessage(std::string Message) {
   this->Log.push_back(Message);
   while (this->Log.size() > 50) {
      this->Log.erase(this->Log.begin());
   }
}

std::vector<std::string> GameState::GetLogMessages() {
   return this->Log;
}

std::shared_ptr<Landmark> GameState::GetCurrentLandmark() {
   if (this->CurrentLandmarkIndex < 0) {
      return nullptr;
   }

   if (this->CurrentLandmarkIndex > this->Landmarks.size()) {
      return nullptr;
   }

   return std::shared_ptr<Landmark>(this->Landmarks.at(this->CurrentLandmarkIndex));


}

std::shared_ptr<Landmark> GameState::GeneratePlayerFarm() {
   // This needs to be moved into its own module at some point...
   std::uniform_int_distribution<int> grassTuftDistrobution(0, 10000);

   auto farmName = TextGenerator::GenerateFarmName();
   auto result = Landmark::Construct(farmName, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT);

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
         } else if (n <= (permille_chance += 10)) {
            result->SetTile(x, y, TileStone);
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

   return result;
}

int GameState::GetPlayerX() {
   return this->PlayerX;
}

int GameState::GetPlayerY() {
   return this->PlayerY;
}

void GameState::WalkPlayer(eDirection direction) {
   auto currentLandmark = this->GetCurrentLandmark();
   switch (direction) {
      case DirectionUp:
         if (this->PlayerY == 0)
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PlayerX, this->PlayerY - 1), Walkable)) {
            std::stringstream str;
            str << "You are blocked by a ";
            str << currentLandmark->GetTile(this->PlayerX, this->PlayerY - 1).Name;
            str << "..";
            this->AddLogMessage(str.str());
            return;
         }
         this->PlayerY--;
         break;

      case DirectionDown:
         if (this->PlayerY == (currentLandmark->GetHeight() - 1))
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PlayerX, this->PlayerY + 1), Walkable)) {
            std::stringstream str;
            str << "You are blocked by a ";
            str << currentLandmark->GetTile(this->PlayerX, this->PlayerY + 1).Name;
            str << "..";
            this->AddLogMessage(str.str());
            return;
         }
         this->PlayerY++;
         break;

      case DirectionLeft:
         if (this->PlayerX == 0)
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PlayerX - 1, this->PlayerY), Walkable)) {
            std::stringstream str;
            str << "You are blocked by a ";
            str << currentLandmark->GetTile(this->PlayerX - 1, this->PlayerY).Name;
            str << "..";
            this->AddLogMessage(str.str());
            return;
         }
         this->PlayerX--;
         break;

      case DirectionRight:
         if (this->PlayerX == (currentLandmark->GetWidth() - 1))
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PlayerX + 1, this->PlayerY), Walkable)) {
            std::stringstream str;
            str << "You are blocked by a ";
            str << currentLandmark->GetTile(this->PlayerX + 1, this->PlayerY).Name;
            str << "..";
            this->AddLogMessage(str.str());
            return;
         }
         this->PlayerX++;
         break;
   }

}
