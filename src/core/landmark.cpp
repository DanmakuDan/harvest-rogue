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
   this->Items = new LandmarkItem[width * height];
   
   for (auto i = 0; i < width * height; i++) {
      this->Tiles.push_back(Tile::FromTileType(TileType::Nothing));
      this->Items[i] = {};
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

void Landmark::SetTile(int x, int y, TileType::TileType tile) {
   auto index = x + (y * this->GetWidth());
   if ((index < 0) || (index >= this->Tiles.size())) {
      return;
   }
   this->Tiles[index] = Tile::FromTileType(tile);
}

Tile::Tile Landmark::GetTile(int x, int y) {
   auto index = x + (y * this->GetWidth());
   if ((index < 0) || (index >= this->Tiles.size())) {
      throw;
   }
   return this->Tiles[index];
}

void Landmark::AddItem(int x, int y, std::shared_ptr<Item> item) {
   if (this->GetItem(x, y) != nullptr) {
      throw;
   }
   this->Items[x + (y * Width)] = { x, y, std::shared_ptr<Item>(item) };
}

std::shared_ptr<Item> Landmark::GetItem(int x, int y) {
   return this->Items[x + (y * Width)].Item;
}

void Landmark::RemoveItem(int x, int y) {
   this->Items[x + (y * Width)] = {};
}

bool Landmark::LocateItem(std::shared_ptr<Item> item, int &x, int &y) {
   x = 0;
   y = 0;

   if (item == nullptr) {
      return false;
   }

   for (int i = 0; i < (Width * Height); i++) {
      auto mapItem = this->Items[i];
      if (mapItem.Item != item) {
         continue;
      }

      x = mapItem.x;
      y = mapItem.y;
      return true;
   }

   return false;
}

LandmarkItem* Landmark::GetAllLandmarkItems() {
   return this->Items;
}
