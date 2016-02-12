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


#include "actiondirectiondialog.h"
#include "screen.h"
#include "input.h"
#include "direction.h"
#include "gamestate.h"

ActionDirectionDialog::ActionDirectionDialog(std::shared_ptr<Item> targetTool) {
   this->TargetTool = std::shared_ptr<Item>(targetTool);
}

void ActionDirectionDialog::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MoveUp)) {
      this->TargetTool->Use(Direction::Up);
      GameState::Get().PopDialog();
      return;
   }

   if (Action::Requested(action, Action::MoveDown)) {
      this->TargetTool->Use(Direction::Down);
      GameState::Get().PopDialog();
      return;
   }

   if (Action::Requested(action, Action::MoveLeft)) {
      this->TargetTool->Use(Direction::Left);
      GameState::Get().PopDialog();
      return;
   }

   if (Action::Requested(action, Action::MoveRight)) {
      this->TargetTool->Use(Direction::Right);
      GameState::Get().PopDialog();
      return;
   }

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().PopDialog();
      return;
   }

}

void ActionDirectionDialog::Render() {
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (ACTION_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (ACTION_DIALOG_HEIGHT / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, ACTION_DIALOG_WIDTH, ACTION_DIALOG_HEIGHT, "Action Direction");
   Screen::Get().WriteText(dialogLeft + 1, dialogTop + 1, "Press a movement direction.");
}
