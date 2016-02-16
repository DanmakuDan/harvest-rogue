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
      this->Tiles.push_back(Tile::FromTileType(TileType::Nothing));
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

void Landmark::AddItem(int x, int y, ItemPtr item) {
   if (this->GetItem(x, y) != nullptr) {
      throw;
   }

   LandmarkItem landmarkItem = { x, y, ItemPtr(item) };
   this->Items[x + (y * Width)] = std::make_shared<LandmarkItem>(landmarkItem);
}

ItemPtr Landmark::GetItem(int x, int y) {
   auto result = this->Items[x + (y * Width)];
   if (result == nullptr) {
      return nullptr;
   }
   return result->ItemTarget;
}

void Landmark::RemoveItem(int x, int y) {
   this->Items.erase(x + (y * Width));
}

bool Landmark::LocateItem(ItemPtr item, int &x, int &y) {
   x = 0;
   y = 0;

   if (item == nullptr) {
      return false;
   }

   for(auto i: this->Items) {
      auto mapItem = i.second;
      if (mapItem == nullptr) {
         continue;
      }
      if (mapItem->ItemTarget != item) {
         continue;
      }

      x = mapItem->x;
      y = mapItem->y;
      return true;
   }

   return false;
}

std::map<int, std::shared_ptr<LandmarkItem>> Landmark::GetAllLandmarkItems() {
   return this->Items;
}
