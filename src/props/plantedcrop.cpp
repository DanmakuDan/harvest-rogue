///*
//    harvest-rogue is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    harvest-rogue is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with harvest-rogue.  If not, see <http://www.gnu.org/licenses/>.     */
//
//#include <common/crops.h>
//#include <sstream>
//#include "plantedcrop.h"
//#include "gamestate.h"
//
//PlantedCrop::PlantedCrop(Crop::Crop crop, CropGrowthType::CropGrowthType cropGrowthType, int hoursLeftToGrow) {
//   this->Crop = crop;
//   this->CropGrowthType = cropGrowthType;
//   if (hoursLeftToGrow == -1) {
//      this->HoursLeftToGrow = crop.HoursToGrow;
//   } else {
//      this->HoursLeftToGrow = hoursLeftToGrow;
//   }
//}
//
//std::string PlantedCrop::GetName() {
//   std::stringstream result;
//   result << CropGrowthType::GetName(this->CropGrowthType);
//   result << " ";
//   result << this->Crop.Name;
//   return result.str();
//}
//
//std::string PlantedCrop::GetDescription() {
//   return GetName();
//}
//
//TileType::TileType PlantedCrop::GetTileType() {
//   auto it = this->Crop.TileTypes.find(this->CropGrowthType);
//
//   if (it == this->Crop.TileTypes.end()) {
//      return TileType::Nothing;
//   } else {
//      return it->second;
//   }
//}
//
//bool PlantedCrop::Takeable() {
//   return false;
//}
//
//void PlantedCrop::OnHourlyTick() {
//   if (this->CropGrowthType == CropGrowthType::Wilted) {
//      return;
//   }
//
//   this->HoursLeftToGrow--;
//
//   if (this->HoursLeftToGrow <= -18) {
//      this->CropGrowthType = CropGrowthType::Wilted;
//      return;
//   }
//
//   if ((this->HoursLeftToGrow > 0) && (this->CropGrowthType == CropGrowthType::Seedling)) {
//      this->CropGrowthType = CropGrowthType::Growing;
//      return;
//   }
//
//   if ((this->HoursLeftToGrow <= 0) && (this->CropGrowthType == CropGrowthType::Growing)) {
//      this->CropGrowthType = CropGrowthType::FullyGrown;
//   }
//}
//
//void PlantedCrop::Interact() {
//   auto currentLandmark = GameState::Get().GetCurrentLandmark();
//   int x, y;
//   if (!currentLandmark->LocateItem(this->shared_from_this(), x, y)) {
//      throw;
//   }
//
//   currentLandmark->RemoveItem(x, y);
//}
