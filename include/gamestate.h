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

#ifndef HARVEST_ROGUE_GAMESTATE_H
#define HARVEST_ROGUE_GAMESTATE_H

#include "scene.h"
#include <memory>
#include <string>
#include <sstream>
#include <iostream>

enum eGameStateSeason {
   SeasonSpring,
   SeasonSummer,
   SeasonFall,
   SeasonWinter
};

const std::string eGameStateSeasonDescs[] = {"Spring", "Summer", "Fall", "Winter"};

class GameState {
private:
   GameState();

   GameState(GameState const &) { };

   GameState &operator=(GameState const &) { };

public:
   ~GameState();

   static GameState &Get() {
      static GameState gameState;
      return gameState;
   }

   std::shared_ptr<IScene> GetCurrentScene();

   void SetCurrentScene(std::shared_ptr<IScene> newScene);

   bool IsActive();

   void Terminate();

   bool TransitionScene();

   void InitializeNewGame();

   void StepSimulation();

   int GetCurrentDay();

   eGameStateSeason GetCurrentSeason();

   int GetCurrentYear();

   int GetCurrentHour();

   int GetCurrentMinute();

   int GetCurrentSecond();

private:
   bool active;
   std::shared_ptr<IScene> CurrentScene;
   std::shared_ptr<IScene> NextScene;

   eGameStateSeason CurrentSeason;
   int CurrentDay, CurrentYear, CurrentHour, CurrentMinute, CurrentSecond;
};


#endif //HARVEST_ROGUE_GAMESTATE_H
