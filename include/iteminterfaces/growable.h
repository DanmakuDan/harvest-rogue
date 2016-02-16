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

#ifndef HARVEST_ROGUE_GROWABLE_H
#define HARVEST_ROGUE_GROWABLE_H


#include "iteminterface.h"
#include "crops.h"
#include "tickevents.h"
#include <memory>

struct GrowableTileCode {
   Color::Color ColorCode;
   char CharacterCode;
   int GfxTileCode;
   SurfaceAttribute::SurfaceAttribute SurfaceAttributes;
};

class Growable : public IItemInterface, public IHourlyTickEvent {
public:
   IItemInterface* Clone() const { return new Growable(*this); }
private:
   Growable();

   Growable(Growable const &src) {
      this->HoursToGrow = src.HoursToGrow;
      this->HoursToWilt = src.HoursToWilt;
      this->Seedling = src.Seedling;
      this->Growing = src.Growing;
      this->Grown = src.Grown;
      this->Wilted = src.Wilted;
   };
public:
   ~Growable();
   static std::shared_ptr<Growable> Deserialize(picojson::value serializedValue);
   
   void SetHoursToGrow(int hoursToGrow);
   int GetHoursToGrow();
   void SetHoursToWilt(int hoursToWilt);
   int GetHoursToWilt();
   void SetSeedlingGrowableTileCode(GrowableTileCode growableTileCode);
   GrowableTileCode GetSeedlingGrowableTileCode();
   void SetGrowingGrowableTileCode(GrowableTileCode growableTileCode);
   GrowableTileCode GetGrowingGrowableTileCode();
   void SetGrownGrowableTileCode(GrowableTileCode growableTileCode);
   GrowableTileCode GetGrownGrowableTileCode();
   void SetWiltedGrowableTileCode(GrowableTileCode growableTileCode);
   GrowableTileCode GetWiltedGrowableTileCode();
   void SetCropGrowthType(CropGrowthType::CropGrowthType cropGrowthType);
   CropGrowthType::CropGrowthType GetCropGrowthType();
   

   void StartGrowing(ItemPtr sourceItem);
   bool IsFullyGrown();

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();

   // IHourlyTickEvent
   virtual void OnHourlyTick(ItemPtr sourceItem);
private:
   int HoursToGrow;
   int HoursToWilt;
   GrowableTileCode Seedling;
   GrowableTileCode Growing;
   GrowableTileCode Grown;
   GrowableTileCode Wilted;
   CropGrowthType::CropGrowthType CropGrowthType;
   static void ApplyGrowableTileCode(ItemPtr sourceItem, GrowableTileCode growableTileCode);
   static GrowableTileCode ParseGrowableTileCode(picojson::value serializedValue);
   static SurfaceAttribute::SurfaceAttribute ParseSurfaceAttributes(picojson::array surfaceAttributes);
};

#endif //HARVEST_ROGUE_GROWABLE_H
