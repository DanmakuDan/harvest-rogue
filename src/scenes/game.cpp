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

#include "tiles.h"
#include <sstream>
#include "input.h"
#include "gamestate.h"
#include "player.h"
#include "game.h"
#include "gamemenudialog.h"
#include "actiondialog.h"
#include "inventorydialog.h"
#include "screen.h"
#include "iteminterfacetype.h"
#include "obtainable.h"

#ifdef WIN32
#include "windowsshim.h"
#endif

void Game::InitializeScreen() {
   Input::Get().SetInputTimeout(50);
   this->RenderUI();
}

void Game::OnKeyPress(int key) {

   auto currentDialog = GameState::Get().GetCurrentDialog();
   if (currentDialog != nullptr) {
      currentDialog->OnKeyPress(key);

      this->RenderUI();
      // Don't step simulation when in dialogs
      return;
   }

   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::GameMenu)) {
      GameState::Get().PushDialog(GameMenuDialog::Construct());
   }

   if (Action::Requested(action, Action::PickUpItemFromGround)) {
      Player::Get().PickUpItemFromGround();
   }

   if (Action::Requested(action, Action::MoveUp)) {
      Player::Get().WalkPlayer(Direction::Up);
   }

   if (Action::Requested(action, Action::MoveDown)) {
      Player::Get().WalkPlayer(Direction::Down);
   }

   if (Action::Requested(action, Action::MoveLeft)) {
      Player::Get().WalkPlayer(Direction::Left);
   }

   if (Action::Requested(action, Action::MoveRight)) {
      Player::Get().WalkPlayer(Direction::Right);
   }

   if (Action::Requested(action, Action::UseTool)) {
      Player::Get().UseEquippedItem();
   }

   if (Action::Requested(action, Action::OpenActionList)) {
      GameState::Get().PushDialog(ActionDialog::Construct());
   }

   if (Action::Requested(action, Action::OpenInventory)) {
      GameState::Get().PushDialog(InventoryDialog::Construct(Player::Get().AsItemContainer()));
   }

   if (Action::Requested(action, Action::InteractWithObject)) {
      Player::Get().InteractWith();
   }

   GameState::Get().StepSimulation();
   this->RenderUI();
}

Game::Game() {

}

void Game::RenderUI() {
   Screen::Get().BeginScreenUpdate();

   this->RenderTopBar();
   this->RenderMap();
   this->RenderLog();
   this->RenderStatusBar();
   this->RenderSideBar();

   auto currentDialog = GameState::Get().GetCurrentDialog();
   if (currentDialog != nullptr) {
      currentDialog->Render();
   }

   Screen::Get().EndScreenUpdate();
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

   Screen::Get().ClearLine(0, Color::Inverse(Color::White));
   Screen::Get().WriteText(0, 0, topStatusBar, Color::Inverse(Color::White));

   free(topStatusBar);
}

void Game::RenderMap() {
   // Screen rendering offsets
   auto startX = 1;
   auto startY = 1;

   auto playerX = Player::Get().GetPositionX();
   auto playerY = Player::Get().GetPositionY();

   auto drawWidth = Screen::Get().GetWidth() - GAME_UI_MAP_PADDING_RIGHT;
   auto drawHeight = Screen::Get().GetHeight() - GAME_UI_MAP_PADDING_BOTTOM;
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto totalMapWidth = currentLandmark->GetWidth();
   auto totalMapHeight = currentLandmark->GetHeight();

   // Map rendering offsets, centered on player
   auto mapOffsetX = playerX - (drawWidth / 2);
   auto mapOffsetY = playerY - (drawHeight / 2);

   // Don't go beyond map boundaries
   if (mapOffsetX + drawWidth > (int)totalMapWidth)
      mapOffsetX = (int)totalMapWidth - (drawWidth - 1);
   if (mapOffsetY + drawHeight > (int)totalMapHeight)
      mapOffsetY = (int)totalMapHeight - (drawHeight - 1);

   mapOffsetX = mapOffsetX < 0 ? 0 : mapOffsetX;
   mapOffsetY = mapOffsetY < 0 ? 0 : mapOffsetY;

   // Draw loop
   for (auto mapY = 0; mapY < drawHeight - startY && mapY <= (int)totalMapHeight - startY; mapY++) {
      for (auto mapX = 0; mapX < drawWidth - startX && mapX <= (int)totalMapWidth - startX; mapX++) {


         auto groundTile = currentLandmark->GetTile(mapX + mapOffsetX, mapY + mapOffsetY);
         Screen::Get().WriteTile(mapX + startX, mapY + startY, groundTile.GfxTileCode, groundTile.CharacterCode, groundTile.ColorCode);


         auto mapProp = currentLandmark->GetItem(mapX + mapOffsetX, mapY + mapOffsetY);
         if (mapProp != nullptr) {
            Screen::Get().WriteTile(mapX + startX, mapY + startY, mapProp->GetGfxTileCode(), mapProp->GetCharacterCode(), mapProp->GetColorCode());
         }

         if ((mapX + mapOffsetX == playerX) && (mapY + mapOffsetY == playerY)) {
            // Draw the player
            auto playerTile = Tile::FromTileType(TileType::Player);
            Screen::Get().WriteTile(mapX + startX, mapY + startY, playerTile.GfxTileCode, playerTile.CharacterCode, playerTile.ColorCode);
         } 
      }
   }
}

void Game::RenderLog() {
   auto logMessageOutputTop = Screen::Get().GetHeight() - (GAME_UI_MAP_PADDING_BOTTOM - 1);
   auto logMessages = GameState::Get().GetLogMessages();
   auto logMessageCount = logMessages.size();
   auto startIndex = (logMessageCount < 6) ? 0 : logMessageCount - 6;

   for (auto i = 0; (i < 6) && ((startIndex + i) < logMessageCount); i++) {
      Screen::Get().ClearLine(logMessageOutputTop + i, Color::Cyan);
      Screen::Get().WriteText(1, logMessageOutputTop + i, logMessages[startIndex + i], Color::Cyan);
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

   Screen::Get().ClearLine(statusBarLine, Color::Inverse(Color::White));
   Screen::Get().WriteText(1, statusBarLine, statusBarText.str(), Color::Inverse(Color::Blue));
}

void Game::RenderSideBar() {
   auto sideBarLeft = (Screen::Get().GetWidth() - (GAME_UI_MAP_PADDING_RIGHT)) + 1;
   auto currentLandmark = GameState::Get().GetCurrentLandmark();

   Screen::Get().WriteText(sideBarLeft, 3, "Feeling ", Color::Silver);
   auto playerEnergy = Player::Get().GetEnergy();
   if (playerEnergy <= ENERGY_EXAUSTED) {
      Screen::Get().WriteText(sideBarLeft + 8, 3, "EXAUSTED", Color::BrightRed);
   } else if (playerEnergy <= ENERGY_TIRED) {
      Screen::Get().WriteText(sideBarLeft + 8, 3, "tired", Color::BrightYellow);
   }else if (playerEnergy <= ENERGY_GOOD) {
      Screen::Get().WriteText(sideBarLeft + 8, 3, "good", Color::White);
   } else  {
      Screen::Get().WriteText(sideBarLeft + 8, 3, "well rested", Color::BrightGreen);
   }

   Screen::Get().WriteText(sideBarLeft, 4, "Standing on", Color::Silver);
   auto currentProp = currentLandmark->GetItem(Player::Get().GetPositionX(), Player::Get().GetPositionY());
   if (currentProp == nullptr) {
      auto currentTile = currentLandmark->GetTile(Player::Get().GetPositionX(), Player::Get().GetPositionY());
      Screen::Get().WriteText(sideBarLeft + 12, 4, currentTile.Name, currentTile.ColorCode);
   } else {
      Screen::Get().WriteText(sideBarLeft + 12, 4, currentProp->GetName(), currentProp->GetColorCode());
   }

   auto currentTool = Player::Get().GetCurrentlyEquippedItem();
   Screen::Get().WriteText(sideBarLeft, 5, "Holding", Color::Silver);
   std::stringstream currentToolStr;
   if (currentTool == nullptr) {
      currentToolStr << "Nothing";
   }
   else {
      currentToolStr << currentTool->GetName();

      auto obtainableInterface = currentTool->GetInterface<Obtainable>(ItemInterfaceType::Obtainable);
      if (obtainableInterface->GetIsStackable()) {
         currentToolStr << " (";
         currentToolStr << currentTool->GetCount();
         currentToolStr << ")";
      }
   }
   Screen::Get().WriteText(sideBarLeft + 8, 5, currentToolStr.str(), Color::Gray);

}
