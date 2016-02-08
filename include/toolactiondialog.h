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

#ifndef HARVEST_ROGUE_TOOLACTIONDIALOG_H
#define HARVEST_ROGUE_TOOLACTIONDIALOG_H

#include <memory>
#include "dialog.h"
#include "tool.h"

#define TOOLACTIN_DIALOG_WIDTH  35

enum eToolActionDialogOptions {
   OptEquipTool = 0,
   OptDropTool,
   eToolActionDialogOptionsMax
};

class ToolActionDialog : public IDialog {
public:
   static std::shared_ptr<ToolActionDialog> Construct(std::shared_ptr<ITool> tool) {
      return std::shared_ptr<ToolActionDialog>(new ToolActionDialog(tool));
   }
   virtual void OnKeyPress(int key);
   virtual void Render();
private:
   ToolActionDialog(std::shared_ptr<ITool> tool);
   std::shared_ptr<ITool> Tool;
   eToolActionDialogOptions SelectedOption;

   void ExecuteSelectedAction();
};


#endif //HARVEST_ROGUE_TOOLACTIONDIALOG_H
