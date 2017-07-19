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

#ifndef HARVEST_ROGUE_GAME_H
#define HARVEST_ROGUE_GAME_H

#include "scene.h"
#include <memory>

#define GAME_UI_MAP_PADDING_RIGHT  30
#define GAME_UI_MAP_PADDING_BOTTOM 9

class Game : public IScene {
public:
   void InitializeScreen() override;

   void OnKeyPress(int key) override;

   static std::shared_ptr<Game> Construct() {
      return std::shared_ptr<Game>(new Game());
   }

private:
   Game();

   static void RenderUI();

   static void RenderTopBar();

   static void RenderMap();

   static void RenderLog();

   static void RenderStatusBar();

   static void RenderSideBar();
};


#endif //HARVEST_ROGUE_GAME_H
