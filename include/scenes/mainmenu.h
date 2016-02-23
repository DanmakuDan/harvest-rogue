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

#ifndef HARVEST_ROGUE_MAINMENU_H
#define HARVEST_ROGUE_MAINMENU_H

#include "scene.h"
#include <memory>

class MainMenu : public IScene {
public:
   void InitializeScreen() override;
   void OnKeyPress(int key) override;

   virtual void Render();

   static std::shared_ptr<MainMenu> Construct() {
      return std::shared_ptr<MainMenu>(new MainMenu());
   }

private:
   MainMenu();

   int SelectedButton;

   void DrawMenu();
};


#endif //HARVEST_ROGUE_MAINMENU_H
