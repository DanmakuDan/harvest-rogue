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

GameState::GameState() {
   this->active = true;
   this->CurrentScene = nullptr;
   this->NextScene = nullptr;

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
}

void GameState::StepSimulation() {
   CurrentSecond += 20;
   if (CurrentSecond > 59) {
      CurrentSecond = 0;
      CurrentMinute++;
      if (CurrentMinute > 59) {
         CurrentMinute = 0;
         CurrentHour++;
         if (CurrentHour > 23) {
            CurrentHour = 0;
            CurrentDay++;
            if (CurrentDay > 30) {
               CurrentDay = 1;
               CurrentSeason = (eGameStateSeason) ((int) CurrentSeason + 1);
               if ((int) CurrentSeason > 3) {
                  CurrentSeason = (eGameStateSeason) 0;
                  CurrentYear++;
               }
            }
         }
      }
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
}

std::vector<std::string> GameState::GetLogMessages() {
   return this->Log;
}
