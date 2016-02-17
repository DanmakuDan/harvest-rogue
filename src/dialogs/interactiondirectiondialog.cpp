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

#include "interactiondirectiondialog.h"
#include "input.h"
#include "screen.h"
#include "gamestate.h"
#include "direction.h"
#include "player.h"

InteractionDirectionDialog::InteractionDirectionDialog() {

}

void InteractionDirectionDialog::OnKeyPress(int key) {

   auto action = Input::Get().GetActionForKeyPress(key);

   if (Action::Requested(action, Action::MoveUp)) {
      GameState::Get().ClearAllDialogs();
      Player::Get().InteractWith(Direction::Up);
      return;
   }

   if (Action::Requested(action, Action::MoveDown)) {
      GameState::Get().ClearAllDialogs();
      Player::Get().InteractWith(Direction::Down);
      return;
   }

   if (Action::Requested(action, Action::MoveLeft)) {
      GameState::Get().ClearAllDialogs();
      Player::Get().InteractWith(Direction::Left);
      return;
   }

   if (Action::Requested(action, Action::MoveRight)) {
      GameState::Get().ClearAllDialogs();
      Player::Get().InteractWith(Direction::Right);
      return;
   }

   if (Action::Requested(action, Action::MenuCancel)) {
      GameState::Get().ClearAllDialogs();
      return;
   }

}

void InteractionDirectionDialog::Render() {
   auto dialogLeft = (Screen::Get().GetWidth() / 2) - (INTERACTION_DIRECTION_DIALOG_WIDTH / 2);
   auto dialogTop = (Screen::Get().GetHeight() / 2) - (INTERACTION_DIRECTION_DIALOG_HEIGHT / 2);
   Screen::Get().WriteWindow(dialogLeft, dialogTop, INTERACTION_DIRECTION_DIALOG_WIDTH,
                             INTERACTION_DIRECTION_DIALOG_HEIGHT, "Interaction Direction");
   Screen::Get().WriteText(dialogLeft + 1, dialogTop + 1, "Press a movement direction.");
}