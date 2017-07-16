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

#ifndef HARVEST_ROGUE_ACTIONDIALOG_H
#define HARVEST_ROGUE_ACTIONDIALOG_H

#include <memory>
#include "dialog.h"

#define ACTION_DIALOG_WIDTH  25
#define ACTION_DIALOG_HEIGHT 5

namespace ActionDialogOption {
   enum ActionDialogOption {
      PickUp = 0,
      Unequip,
      InteractWith,
      _MAX
   };
}

/*
 * Test
 */
class ActionDialog : public IDialog {
public:
   static std::shared_ptr<ActionDialog> Construct() {
      return std::shared_ptr<ActionDialog>(new ActionDialog());
   }

   // IDialog
   void OnKeyPress(int key) override;
   void Render() override;
private:
   ActionDialog();
   ActionDialogOption::ActionDialogOption SelectedOption;
   void ExecuteSelectedAction();
};


#endif //HARVEST_ROGUE_ACTIONDIALOG_H
