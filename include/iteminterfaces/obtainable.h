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

#ifndef HARVEST_ROGUE_OBTAINABLE_H
#define HARVEST_ROGUE_OBTAINABLE_H

#include "iteminterface.h"
#include <memory>

class Obtainable : public IItemInterface {
public:
   IItemInterface* Clone() const override;
private:
   Obtainable();

   Obtainable(Obtainable const &src) {
      this->Stackable = src.Stackable;
      this->MaxStackSize = src.MaxStackSize;
   };

   Obtainable &operator=(Obtainable const &) = delete;
public:
   ~Obtainable();
   static std::shared_ptr<Obtainable> Deserialize(picojson::value serializedValue);

   bool GetIsStackable() const;
   void SetIsStackable(bool stackable);
   int GetMaxStackSize() const;
   void SetMaxStackSize(int maxStackSize);

   // IItemInterface
   ItemInterfaceType::ItemInterfaceType GetInterfaceType() override;
private:
   bool Stackable;
   int MaxStackSize;
};

#endif //HARVEST_ROGUE_OBTAINABLE_H
