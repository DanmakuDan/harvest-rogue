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

#include "itemtransferdialog.h"
#include "input.h"
#include "gamestate.h"
#include "screen.h"

void ItemTransferDialog::OnKeyPress(int key)
{
   auto action = Input::Get().GetActionForKeyPress(key);


   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
   }
}

void ItemTransferDialog::Render()
{
   auto screenCenter = Screen::Get().GetWidth() / 2;
   std::string dialogTitle = "Inventory Transfer";

   Screen::Get().WriteWindow(3, 3, Screen::Get().GetWidth() - 6, 3);
   auto textLeft = screenCenter - (dialogTitle.size() / 2);
   Screen::Get().WriteText(textLeft, 4, dialogTitle);

   // Left Dialog
   Screen::Get().WriteWindow(3, 7, screenCenter - 3, 3);
   auto leftDialogTextWidth = this->FirstContainer->GetName().size();
   Screen::Get().WriteText(3 + ((screenCenter - 3) / 2) - (leftDialogTextWidth / 2), 8, this->FirstContainer->GetName());
   Screen::Get().WriteWindow(3, 10, screenCenter - 3, Screen::Get().GetHeight() - 13);


   // Right dialog
   Screen::Get().WriteWindow(screenCenter, 7, screenCenter - 3, 3);
   auto rightDialogTextWidth = this->FirstContainer->GetName().size();
   Screen::Get().WriteText(screenCenter + ((screenCenter - 3) / 2) - (rightDialogTextWidth / 2), 8, this->SecondContainer->GetName());
   Screen::Get().WriteWindow(screenCenter, 10, screenCenter - 3, Screen::Get().GetHeight() - 13);
   

}

ItemTransferDialog::ItemTransferDialog(ItemContainerPtr firstContainer, ItemContainerPtr secondContainer)
{
   this->FirstContainer = firstContainer;
   this->SecondContainer = secondContainer;
}
