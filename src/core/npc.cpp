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

#include "npc.h"

NPC::NPC() :
   Name("[unknown]"),
   ColorCode(Color::White),
   CharacterCode(0),
   GfxTileCode(0) {}

std::string NPC::GetDescription() const {
   return this->Description;
}

char NPC::GetCharacterCode() const {
   return this->CharacterCode;
}

int NPC::GetGfxTileCode() const {
   return this->GfxTileCode;
}

Color::Color NPC::GetColorCode() const {
   return this->ColorCode;
}

std::string NPC::GetName() {
   return this->Name;
}
