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

#ifndef HARVEST_ROGUE_RESTABLE_H
#define HARVEST_ROGUE_RESTABLE_H


#include "iteminterface.h"
#include "crops.h"
#include "tickevents.h"
#include "interactable.h"
#include <memory>


class Restable : public IItemInterface, public IInteractable {
public:
   IItemInterface* Clone() const { return new Restable(*this); }
private:
   Restable();

   Restable(Restable const &src) { };
public:
   ~Restable();
   static std::shared_ptr<Restable> Deserialize(picojson::value serializedValue);

   // IItemInterface
   virtual ItemInterfaceType::ItemInterfaceType GetInterfaceType();

   // IInteractable
   void Interact(ItemPtr sourceItem);
};

#endif //HARVEST_ROGUE_RESTABLE_H
