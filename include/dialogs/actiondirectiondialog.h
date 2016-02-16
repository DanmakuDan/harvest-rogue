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


#ifndef HARVEST_ROGUE_ACTIONDIRECTIONDIALOG_H
#define HARVEST_ROGUE_ACTIONDIRECTIONDIALOG_H

#include "dialog.h"
#include "item.h"
#include <memory>

#define ACTION_DIALOG_WIDTH      30
#define ACTION_DIALOG_HEIGHT      3

class ActionDirectionDialog : public IDialog {
public:
   static std::shared_ptr<ActionDirectionDialog> Construct(ItemPtr targetTool) {
      return std::shared_ptr<ActionDirectionDialog>(new ActionDirectionDialog(targetTool));
   }
   virtual void OnKeyPress(int key);
   virtual void Render();
private:
   ActionDirectionDialog(ItemPtr targetTool);
   ItemPtr TargetTool;
};


#endif //HARVEST_ROGUE_ACTIONDIRECTIONDIALOG_H
