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

#include "gamestate.h"
#include "input.h"
#include "screen.h"
#include "craftingdialog.h"

void CraftingDialog::OnKeyPress(int key) {
   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }
}

void CraftingDialog::Render() {
   this->DrawDialogHeader(3, 3, Screen::Get().GetWidth() - 6, 3);
}

void CraftingDialog::DrawDialogHeader(int x, int y, int width, int height) {
   std::string dialogTitle = "Crafting";

   Screen::Get().WriteWindow(x, y, width, height);
   auto textLeft = x + (width / 2) - (dialogTitle.size() / 2);
   Screen::Get().WriteText(textLeft, y + 1, dialogTitle);
}
