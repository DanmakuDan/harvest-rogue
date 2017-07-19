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

#ifndef HARVEST_ROGUE_GAMEMENUDIALOG_H
#define HARVEST_ROGUE_GAMEMENUDIALOG_H

#include "dialog.h"
#include <memory>

#define GAMEMENU_DIALOG_WIDTH  30

namespace GameMenuDialogOption {
   enum GameMenuDialogOption {
      Status = 0,
      Inventory,
      Actions,
      Craft,
      SaveGame,
      Quit,
      _MAX
   };
}


class GameMenuDialog : public IDialog {
public:
   static std::shared_ptr<GameMenuDialog> Construct() {
      return std::shared_ptr<GameMenuDialog>(new GameMenuDialog());
   }

   // IDialog
   void OnKeyPress(int key) override;
   void Render() override;

private:
   GameMenuDialog();
   GameMenuDialogOption::GameMenuDialogOption SelectedOption;
   void ExecuteSelectedAction() const;
};


#endif //HARVEST_ROGUE_GAMEMENUDIALOG_H
