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

#ifndef HARVEST_ROGUE_CRAFTINGDIALOG_H
#define HARVEST_ROGUE_CRAFTINGDIALOG_H

#include "dialog.h"
#include <map>

class CraftingDialog : public IDialog {
public:
   static std::shared_ptr<CraftingDialog> Construct() {
      return std::shared_ptr<CraftingDialog>(new CraftingDialog());
   }

   // IDialog
   void OnKeyPress(int key) override;
   void Render() override;

private:
   CraftingDialog();
   void SelectPreviousCategory();
   void SelectNextCategory();
   void SelectNextItem();
   void SelectPreviousItem();
   void DrawDialogHeader(int x, int y, int width, int height);
   void DrawNavigationDialog();
   void DrawDetailsDialog();
   void AddItemToCategory(std::string category, ItemPtr item);
   std::map<std::string, std::vector<ItemPtr>> ItemCategories;
   std::string CurrentCategory;
   ItemPtr CurrentItem;
};


#endif //HARVEST_ROGUE_CRAFTINGDIALOG_H
