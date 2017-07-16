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

#ifndef HARVEST_ROGUE_CRAFTINGCONFIRMDIALOG_H
#define HARVEST_ROGUE_CRAFTINGCONFIRMDIALOG_H

#include "dialog.h"
#include "item.h"
#include <memory>
#include <string>

#define CRAFTINGCONFIRM_DIALOG_WIDTH      50
#define CRAFTINGCONFIRM_DIALOG_MAX_CRAFT  1000
namespace CraftingConfirmDialogOption {
   enum CraftingConfirmDialogOption {
      RemoveOneHundred,
      RemoveTen,
      RemoveOne,
      Craft,
      AddOne,
      AddTen,
      AddOneHundred,
      _MAX
   };
}
class CraftingConfirmDialog : public IDialog {
public:
   static std::shared_ptr<CraftingConfirmDialog > Construct(ItemPtr itemToConstruct) {
      return std::shared_ptr<CraftingConfirmDialog >(new CraftingConfirmDialog(itemToConstruct));
   }

   // IDialog
   void OnKeyPress(int key) override;
   void Render() override;

private:
   int AmountToCraft;
   ItemPtr ItemToConstruct;
   bool HasMaterialToCraft;
   CraftingConfirmDialogOption::CraftingConfirmDialogOption SelectedOption;
   std::string DialogTitle;
   CraftingConfirmDialog(ItemPtr itemToConstruct);
   void Craft() const;
};

#endif // HARVEST_ROGUE_CRAFTINGCONFIRMDIALOG_H
