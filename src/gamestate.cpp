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

#include "gamestate.h"
#include "textgenerator.h"
#include "landmarkgenerator.h"

GameState::GameState() {
   this->active = true;
   this->CurrentScene = nullptr;
   this->NextScene = nullptr;
   this->CurrentLandmarkIndex = -1;
   this->PlayerX = 5;
   this->PlayerY = 5;

   // Temporary debug stuff
   this->AddLogMessage("Welcome to Harvest-Rogue!");
   this->AddLogMessage("This is a pre-alpha release. Got feedback? Open an issue on github!");
   this->AddLogMessage("https://github.com/essial/harvest-rogue");


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
   this->Landmarks.push_back(LandmarkGenerator::GeneratePlayerFarm());
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

void GameState::AddLogMessageFmt(const std::string format, ...) {
   int size = 4096;
   std::string str;
   va_list ap;
   while (1) {     // Maximum two passes on a POSIX system...
      str.resize(size);
      va_start(ap, format);
      int n = vsnprintf((char *) str.data(), size, format.c_str(), ap);
      va_end(ap);
      if (n > -1 && n < size) {  // Everything worked
         str.resize(n);
         this->AddLogMessage(str);
         return;
      }
      if (n > -1)  // Needed size returned
         size = n + 1;   // For null char
      else
         size *= 2;      // Guess at a larger size (OS specific)
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
            this->AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PlayerX, this->PlayerY - 1).Name.c_str());
            return;
         }
         this->PlayerY--;
         break;

      case DirectionDown:
         if (this->PlayerY == (currentLandmark->GetHeight() - 1))
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PlayerX, this->PlayerY + 1), Walkable)) {
            this->AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PlayerX, this->PlayerY + 1).Name.c_str());
            return;
         }
         this->PlayerY++;
         break;

      case DirectionLeft:
         if (this->PlayerX == 0)
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PlayerX - 1, this->PlayerY), Walkable)) {
            this->AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PlayerX - 1, this->PlayerY).Name.c_str());
            return;
         }
         this->PlayerX--;
         break;

      case DirectionRight:
         if (this->PlayerX == (currentLandmark->GetWidth() - 1))
            return;
         if (!TileHasSurfaceAttribute(currentLandmark->GetTile(this->PlayerX + 1, this->PlayerY), Walkable)) {
            this->AddLogMessageFmt("You are blocked by '%s'..",
                                   currentLandmark->GetTile(this->PlayerX + 1, this->PlayerY).Name.c_str());
            return;
         }
         this->PlayerX++;
         break;
   }

}

std::shared_ptr<IDialog> GameState::GetCurrentDialog() {
   if (this->DialogStack.empty()) {
      return nullptr;
   }
   return this->DialogStack.back();
}

void GameState::PushDialog(std::shared_ptr<IDialog> dialog) {
   this->DialogStack.push_back(std::shared_ptr<IDialog>(dialog));
}

void GameState::PopDialog() {
   if (this->DialogStack.empty()) {
      return;
   }
   this->DialogStack.pop_back();
}
