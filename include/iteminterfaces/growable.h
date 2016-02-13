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
#include <memory>

struct GrowableTileCode {
   Color::Color ColorCode;
   char CharacterCode;
   int GfxTileCode;
};

class Growable : public IItemInterface {
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
   
   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();
private:
   int HoursToGrow;
   int HoursToWilt;
   GrowableTileCode Seedling;
   GrowableTileCode Growing;
   GrowableTileCode Grown;
   GrowableTileCode Wilted;
};

#endif //HARVEST_ROGUE_GROWABLE_H
