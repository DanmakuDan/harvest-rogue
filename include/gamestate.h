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
#include "landmark.h"
#include "dialog.h"
#include "item.h"
#include <memory>
#include <vector>
#include <list>
#include <map>

#define MAP_SIZE_WIDTH 200
#define MAP_SIZE_HEIGHT 200
#define GAMESTATE_SIMULATION_STEP_SECONDS 20
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

   GameState(GameState const &) = delete;
   GameState &operator=(GameState const &) = delete;

public:
   ~GameState();

   static GameState &Get() {
      static GameState gameState;
      return gameState;
   }

   std::shared_ptr<IScene> GetCurrentScene() const;

   void SetCurrentScene(std::shared_ptr<IScene> newScene);

   bool IsActive() const;

   void Terminate();

   bool TransitionScene();

   void InitializeNewGame();

   void StepSimulation();

   int GetCurrentDay() const;

   eGameStateSeason GetCurrentSeason() const;

   int GetCurrentYear() const;

   int GetCurrentHour() const;

   int GetCurrentMinute() const;

   int GetCurrentSecond() const;

   void AddLogMessage(std::string Message);

   void AddLogMessageFmt(const std::string format, ...);

   std::vector<std::string> GetLogMessages() const;

   std::shared_ptr<Landmark> GetCurrentLandmark();

   std::vector<std::shared_ptr<Landmark>> GetAllLandmarks() const;

   std::shared_ptr<IDialog> GetCurrentDialog();

   void PushDialog(std::shared_ptr<IDialog> dialog);

   void ClearAllDialogs();

   void PopDialog();

   void SleepUntilNextMorning(int hour = 6, int minute = 0, int second = 0);

   std::map<std::string, ItemPtr> GetItemDatabase() const;
   ItemPtr GetItemFromItemDatabase(std::string itemName);

private:
   bool active;
   std::shared_ptr<IScene> CurrentScene;
   std::shared_ptr<IScene> NextScene;
   std::vector<std::string> Log;
   std::vector<std::shared_ptr<Landmark>> Landmarks;
   std::list<std::shared_ptr<IDialog>> DialogStack;
   std::map<std::string, ItemPtr> ItemDatabase;
   int CurrentLandmarkIndex;
   eGameStateSeason CurrentSeason;
   int CurrentDay, CurrentYear, CurrentHour, CurrentMinute, CurrentSecond;
   void ProcessDailyTick();
   void ProcessHourlyTick();
};


#endif //HARVEST_ROGUE_GAMESTATE_H
