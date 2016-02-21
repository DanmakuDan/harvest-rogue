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

#include "growable.h"
#include "gamestate.h"

Growable::Growable()
{
}

Growable::~Growable()
{
}


std::shared_ptr<Growable> Growable::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<Growable>(new Growable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "hoursToGrow") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         result->SetHoursToGrow((int)value);
         continue;
      }

      if (item.first == "hoursToWilt") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         result->SetHoursToWilt((int)value);
         continue;
      }

      if (item.first == "seedling") {
         result->SetSeedlingGrowableTileCode(Growable::ParseGrowableTileCode(item.second));
         continue;
      }

      if (item.first == "growing") {
         result->SetGrowingGrowableTileCode(Growable::ParseGrowableTileCode(item.second));
         continue;
      }

      if (item.first == "grown") {
         result->SetGrownGrowableTileCode(Growable::ParseGrowableTileCode(item.second));
         continue;
      }

      if (item.first == "wilted") {
         result->SetWiltedGrowableTileCode(Growable::ParseGrowableTileCode(item.second));
         continue;
      }

      throw;
   }

   return result;
}

void Growable::SetHoursToGrow(int hoursToGrow)
{
   this->HoursToGrow = hoursToGrow;
}

int Growable::GetHoursToGrow()
{
   return this->HoursToGrow;
}

void Growable::SetHoursToWilt(int hoursToWilt)
{
   this->HoursToWilt = hoursToWilt;
}

int Growable::GetHoursToWilt()
{
   return this->HoursToWilt;
}

void Growable::SetSeedlingGrowableTileCode(GrowableTileCode growableTileCode)
{
   this->Seedling = growableTileCode;
}

GrowableTileCode Growable::GetSeedlingGrowableTileCode()
{
   return this->Seedling;
}

void Growable::SetGrowingGrowableTileCode(GrowableTileCode growableTileCode)
{
   this->Growing = growableTileCode;
}

GrowableTileCode Growable::GetGrowingGrowableTileCode()
{
   return this->Growing;
}

void Growable::SetGrownGrowableTileCode(GrowableTileCode growableTileCode)
{
   this->Grown = growableTileCode;
}

GrowableTileCode Growable::GetGrownGrowableTileCode()
{
   return this->Grown;
}

void Growable::SetWiltedGrowableTileCode(GrowableTileCode growableTileCode)
{
   this->Wilted = growableTileCode;
}

GrowableTileCode Growable::GetWiltedGrowableTileCode()
{
   return this->Wilted;
}

void Growable::SetCropGrowthType(CropGrowthType::CropGrowthType cropGrowthType)
{
   this->CropGrowthType = cropGrowthType;
}

CropGrowthType::CropGrowthType Growable::GetCropGrowthType()
{
   return this->CropGrowthType;
}

void Growable::StartGrowing(ItemPtr sourceItem)
{
   Growable::ApplyGrowableTileCode(sourceItem, this->GetSeedlingGrowableTileCode());
   this->SetCropGrowthType(CropGrowthType::Seedling);
}

bool Growable::IsFullyGrown()
{
   return this->GetCropGrowthType() == CropGrowthType::FullyGrown;
}

ItemInterfaceType::ItemInterfaceType Growable::GetInterfaceType()
{
   return ItemInterfaceType::Growable;
}

void Growable::OnHourlyTick(ItemPtr sourceItem)
{
   if (this->GetCropGrowthType() == CropGrowthType::Wilted) {
      return;
   }
   
   if (this->HoursToGrow > 0) {
      this->HoursToGrow--;

      if ((this->HoursToGrow > 0) && (this->GetCropGrowthType() == CropGrowthType::Seedling)) {
         this->SetCropGrowthType(CropGrowthType::Growing);
         Growable::ApplyGrowableTileCode(sourceItem, this->GetGrowingGrowableTileCode());
         sourceItem->SetSurfaceAttributes(this->GetGrowingGrowableTileCode().SurfaceAttributes);
         return;
      }

      if (this->HoursToGrow <= 0) {
         this->SetCropGrowthType(CropGrowthType::FullyGrown);
         Growable::ApplyGrowableTileCode(sourceItem, this->GetGrownGrowableTileCode());
         sourceItem->SetSurfaceAttributes(this->GetGrownGrowableTileCode().SurfaceAttributes);
      }

      return;
   }

      
   if (HoursToWilt > 0) {
      if (--this->HoursToWilt <= 0) {
         this->SetCropGrowthType(CropGrowthType::Wilted);
         Growable::ApplyGrowableTileCode(sourceItem, this->GetWiltedGrowableTileCode());
         sourceItem->SetSurfaceAttributes(this->GetWiltedGrowableTileCode().SurfaceAttributes);
         return;
      }
   }
   
   
}

void Growable::ApplyGrowableTileCode(ItemPtr sourceItem, GrowableTileCode growableTileCode)
{
   sourceItem->SetCharacterCode(growableTileCode.CharacterCode);
   sourceItem->SetColorCode(growableTileCode.ColorCode);
   sourceItem->SetGfxTileCode(growableTileCode.GfxTileCode);
}

GrowableTileCode Growable::ParseGrowableTileCode(picojson::value serializedValue)
{
   GrowableTileCode result = {};

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();

   for (auto item : data) {
      if (item.first == "colorCode") {
         if (!item.second.is<std::string>()) {
            throw;
         }

         auto value = item.second.get<std::string>();
         auto color = Color::FromString(value);
         if (color == Color::Unknown) {
            throw;
         }
         result.ColorCode = color;
         continue;
      }

      if (item.first == "characterCode") {
         if (!item.second.is<std::string>()) {
            throw;
         }

         auto value = item.second.get<std::string>();

         if (value.size() != 1) {
            throw;
         }

         result.CharacterCode = value.front();
         continue;
      }

      if (item.first == "gfxTileCode") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value != (unsigned long)value) {
            throw;
         }

         result.GfxTileCode = (int)value;
         continue;
      }

      if (item.first == "surfaceAttributes") {
         if (!item.second.is<picojson::array>()) {
            throw;
         }

         auto value = item.second.get<picojson::array>();
         result.SurfaceAttributes = Growable::ParseSurfaceAttributes(value);
         continue;
      }

      throw;
   }

   return result;
}

SurfaceAttribute::SurfaceAttribute Growable::ParseSurfaceAttributes(picojson::array surfaceAttributes)
{
   SurfaceAttribute::SurfaceAttribute result = 0;

   for (auto attr : surfaceAttributes) {
      if (!attr.is<std::string>()) {
         throw;
      }
      auto attrName = attr.get<std::string>();
      auto surfaceAttribute = SurfaceAttribute::FromString(attrName);
      if (surfaceAttribute == SurfaceAttribute::Invalid) {
         throw;
      }
      result |= surfaceAttribute;
   }

   return 0;
}
