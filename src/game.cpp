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

#include <input.h>
#include <gamestate.h>
#include <sstream>
#include <iomanip>
#include "game.h"
#include "screen.h"

void Game::InitializeScreen() {
   Screen::Get().Clear();
   Input::Get().SetInputTimeout(50);
   this->RenderUI();
}

void Game::OnKeyPress(int key) {
   GameState::Get().StepSimulation();
   this->RenderUI();
}

Game::Game() {

}

void Game::RenderUI() {
   this->RenderTopBar();
}

void Game::RenderTopBar() {
   std::stringstream topStatusBar;
   topStatusBar << " Harvest-Rogue  ";
   topStatusBar << " Year " << std::setw(2) << GameState::Get().GetCurrentYear() << ", ";
   topStatusBar << " " << std::setw(6) << eGameStateSeasonDescs[GameState::Get().GetCurrentSeason()];
   topStatusBar << " (Day " << std::setw(2) << GameState::Get().GetCurrentDay() << ")";

   auto hour = GameState::Get().GetCurrentHour();
   std::string ampm = "am";
   if (hour == 0) {
      hour = 12;
   } else if (hour == 12) {
      hour = 12;
      ampm = "pm";
   } else if (hour > 11) {
      ampm = "pm";
      hour -= 12;
   }
   topStatusBar << "  " << std::setw(2) << hour;
   topStatusBar << ":" << std::setfill('0') << std::setw(2) << GameState::Get().GetCurrentMinute() << ampm;
   Screen::Get().ClearLine(0, CLR_INVERSE(CLR_WHITE));
   Screen::Get().WriteText(0, 0, topStatusBar.str(), CLR_INVERSE(CLR_WHITE));

}
