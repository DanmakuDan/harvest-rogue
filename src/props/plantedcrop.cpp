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

#include <common/crops.h>
#include <sstream>
#include "plantedcrop.h"

PlantedCrop::PlantedCrop(Crop::Crop crop, CropGrowthType::CropGrowthType cropGrowthType) {
   this->Crop = crop;
   this->CropGrowthType = cropGrowthType;
}

std::string PlantedCrop::GetName() {
   std::stringstream result;
   result << CropGrowthType::GetName(this->CropGrowthType);
   result << " ";
   result << this->Crop.Name;
   return result.str();
}

std::string PlantedCrop::GetDescription() {
   return GetName();
}

TileType::TileType PlantedCrop::GetTileType() {
   auto it = this->Crop.TileTypes.find(this->CropGrowthType);

   if (it == this->Crop.TileTypes.end()) {
      return TileType::Nothing;
   } else {
      return it->second;
   }
}

bool PlantedCrop::Takeable() {
   return false;
}
