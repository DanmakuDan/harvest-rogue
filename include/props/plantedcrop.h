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

#ifndef HARVEST_ROGUE_CROP_H
#define HARVEST_ROGUE_CROP_H

#include "prop.h"
#include "crops.h"
#include "tickevents.h"

class PlantedCrop : public IProp, public IHourlyTickEvent {
public:
   static std::shared_ptr<PlantedCrop> Construct(Crop::Crop crop, CropGrowthType::CropGrowthType cropGrowthType,
                                                 int hoursLeftToGrow = -1) {
      return std::shared_ptr<PlantedCrop>(new PlantedCrop(crop, cropGrowthType, hoursLeftToGrow));
   }

   // IProp
   virtual std::string GetName();
   virtual std::string GetDescription();
   virtual TileType::TileType GetTileType();
   virtual bool Takeable();

   // IHourlyTickEvent
   virtual void OnHourlyTick();

private:
   PlantedCrop(Crop::Crop crop, CropGrowthType::CropGrowthType cropGrowthType, int hoursLeftToGrow);
   Crop::Crop Crop;
   CropGrowthType::CropGrowthType CropGrowthType;
   int HoursLeftToGrow;
};


#endif //HARVEST_ROGUE_CROP_H
