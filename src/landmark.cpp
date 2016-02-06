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

#include "landmark.h"

Landmark::Landmark(std::string name, int width, int height) {
   this->Name = name;
   this->Width = width;
   this->Height = height;
   for (auto i = 0; i < width * height; i++) {
      this->Tiles.push_back(FindTilebyTileType(TileNothing));
   }
}

std::string Landmark::GetName() {
   return this->Name;
}

unsigned int Landmark::GetWidth() {
   return this->Width;
}

unsigned int Landmark::GetHeight() {
   return this->Height;
}

void Landmark::SetTile(int x, int y, eTileType tile) {
   auto index = x + (y * this->GetWidth());
   if ((index < 0) || (index >= this->Tiles.size())) {
      return;
   }
   this->Tiles.at(index) = FindTilebyTileType(tile);
}

Tile Landmark::GetTile(int x, int y) {
   auto index = x + (y * this->GetWidth());
   if ((index < 0) || (index >= this->Tiles.size())) {
      throw;
   }
   return this->Tiles.at(index);
}
