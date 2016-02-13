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

#ifndef HARVEST_ROGUE_PLANTABLE_H
#define HARVEST_ROGUE_PLANTABLE_H


#include "iteminterface.h"
#include "item.h"
#include "useable.h"
#include "equippable.h"
#include <memory>

class Plantable : public IItemInterface, public IUseable, public IEquippable {
public:
   IItemInterface* Clone() const { return new Plantable(*this); }
private:
   Plantable();

   Plantable(Plantable const &src) {
      this->Crop = src.Crop;
   };
public:
   ~Plantable();
   static std::shared_ptr<Plantable> Deserialize(picojson::value serializedValue);

   std::string GetCrop();
   void SetCrop(std::string crop);

   void Plant(std::shared_ptr<Item> sourceItem);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();

   // IUseable
   virtual void Use(std::shared_ptr<Item> sourceItem);

   // IEquippable
   virtual void OnItemEquipped(std::shared_ptr<Item> sourceItem);
   virtual void OnItemUnequipped(std::shared_ptr<Item> sourceItem);
private:
   std::string Crop;
};

#endif //HARVEST_ROGUE_PLANTABLE_H
