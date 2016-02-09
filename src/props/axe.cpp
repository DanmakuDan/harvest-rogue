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

#include "axe.h"

Axe::Axe() {

}

std::string Axe::GetName() {
   return "Hand Axe";
}

std::string Axe::GetDescription() {
   return "A tool for chopping wood";
}

TileType::TileType Axe::GetTileType() {
   return TileType::Axe;
}

bool Axe::Takeable() {
   return true;
}

bool Axe::IsUsable() {
   return true;
}

void Axe::Use() {

}
