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
#include <iomanip>
#include "game.h"

void Game::InitializeScreen() {
   Screen::Get().Clear();
   Input::Get().SetInputTimeout(50);
   this->RenderUI();
}

void Game::OnKeyPress(int key) {
   switch (key) {
      case IK_UP_ARROW:
         GameState::Get().WalkPlayer(DirectionUp);
         break;
      case IK_DOWN_ARROW:
         GameState::Get().WalkPlayer(DirectionDown);
         break;
      case IK_LEFT_ARROW:
         GameState::Get().WalkPlayer(DirectionLeft);
         break;
      case IK_RIGHT_ARROW:
         GameState::Get().WalkPlayer(DirectionRight);
         break;
   }

   GameState::Get().StepSimulation();
   this->RenderUI();
}

Game::Game() {

}

void Game::RenderUI() {
   this->RenderTopBar();
   this->RenderMap();
   this->RenderLog();
   this->RenderStatusBar();
}

void Game::RenderTopBar() {
   auto hour = GameState::Get().GetCurrentHour();
   auto minute = GameState::Get().GetCurrentMinute();

   char *topStatusBar;
   asprintf(&topStatusBar,
            " Harvest-Rogue  Year %2d,  %6s (Day %02d) %2d:%02d%2s",
            GameState::Get().GetCurrentYear(),
            eGameStateSeasonDescs[GameState::Get().GetCurrentSeason()].c_str(),
            GameState::Get().GetCurrentDay(),
            hour % 12,
            minute,
            hour > 11 ? "pm" : "am"
            );

   Screen::Get().ClearLine(0, CLR_INVERSE(CLR_WHITE));
   Screen::Get().WriteText(0, 0, topStatusBar, CLR_INVERSE(CLR_WHITE));

   free(topStatusBar);
}

void Game::RenderMap() {
   auto startX = 1;
   auto startY = 1;
   auto playerX = GameState::Get().GetPlayerX();
   auto playerY = GameState::Get().GetPlayerY();
   auto drawWidth = Screen::Get().GetWidth() - GAME_UI_MAP_PADDING_RIGHT;
   auto drawHeight = Screen::Get().GetHeight() - GAME_UI_MAP_PADDING_BOTTOM;
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto totalMapWidth = currentLandmark->GetWidth();
   auto totalMapHeight = currentLandmark->GetHeight();
   auto mapOffsetX = playerX - (drawWidth / 2);
   auto mapOffsetY = playerY - (drawHeight / 2);

   if ((mapOffsetX + drawWidth) > totalMapWidth) {
      mapOffsetX = totalMapWidth - (drawWidth - 1);
   }
   if (mapOffsetX < 0) {
      mapOffsetX = 0;
   }

   if ((mapOffsetY + drawHeight) > totalMapHeight) {
      mapOffsetY = totalMapHeight - (drawHeight - 1);
   }
   if (mapOffsetY < 0) {
      mapOffsetY = 0;
   }

   for (auto y = startY; y < drawHeight; y++) {
      auto mapY = y - startY;
      if (mapY >= totalMapHeight) {
         continue;
      }
      for (auto x = startX; x < drawWidth; x++) {
         auto mapX = x - startX;
         if (mapX >= totalMapWidth) {
            continue;
         }

         if ((mapX + mapOffsetX == playerX) && (mapY + mapOffsetY == playerY)) {
            auto playerTile = FindTilebyTileType(TilePlayer);
            Screen::Get().WriteTile(x, y, playerTile);
            continue;
         }

         auto tile = currentLandmark->GetTile(mapX + mapOffsetX, mapY + mapOffsetY);
         Screen::Get().WriteTile(x, y, tile);
      }
   }
}

void Game::RenderLog() {
   auto logMessageOutputTop = Screen::Get().GetHeight() - (GAME_UI_MAP_PADDING_BOTTOM - 1);
   auto logMessages = GameState::Get().GetLogMessages();
   auto logMessageCount = logMessages.size();
   auto startIndex = logMessageCount - 6;
   if (startIndex < 0) {
      startIndex = 0;
   }
   for (auto i = 0; (i < 6) && (startIndex + i < logMessageCount); i++) {
      Screen::Get().WriteText(1, logMessageOutputTop + i, logMessages.at(startIndex + i), CLR_CYAN);
   }
}

void Game::RenderStatusBar() {
   auto statusBarLine = Screen::Get().GetHeight() - 1;
   std::stringstream statusBarText;
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   if (currentLandmark == nullptr) {
      statusBarText << "<Unknown Landmark>";
   } else {
      statusBarText << currentLandmark->GetName();
   }

   Screen::Get().ClearLine(statusBarLine, CLR_INVERSE(CLR_WHITE));
   Screen::Get().WriteText(1, statusBarLine, statusBarText.str(), CLR_INVERSE(CLR_BLUE));
}
