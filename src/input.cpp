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

#include "input.h"
#include <ncurses.h>

Input::Input() {
   this->InputTimeout = -1;
}

Input::~Input() {

}
int Input::WaitForAndGetKeyPress() {
   //timeout(this->InputTimeout);
   return wgetch(stdscr);
}

void Input::SetInputTimeout(int timeout) {
   this->InputTimeout = timeout;
}

int Input::GetInputTimeout() {
   return this->InputTimeout;
}

void Input::DisableInputTimeout() {
   this->SetInputTimeout(-1);
}