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

#include "plantable.h"
#include "gamestate.h"
#include "player.h"
#include "growable.h"

IItemInterface* Plantable::Clone() const { return new Plantable(*this); }

Plantable::Plantable() {}

Plantable::~Plantable() {}

std::shared_ptr<Plantable> Plantable::Deserialize(picojson::value serializedValue) {
   auto result = std::shared_ptr<Plantable>(new Plantable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "crop") {
         if (!item.second.is<std::string>()) {
            throw;
         }

         auto value = item.second.get<std::string>();
         result->SetCrop(value);
         continue;
      }

      throw;
   }

   return result;
}

std::string Plantable::GetCrop() const {
   return this->Crop;
}

void Plantable::SetCrop(std::string crop) {
   this->Crop = crop;
}

void Plantable::Plant(ItemPtr sourceItem) const {
   auto x = Player::Get().GetPositionX();
   auto y = Player::Get().GetPositionY();
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   auto currentTile = currentLandmark->GetTile(x, y);

   if (currentTile.TileType != TileType::Tilled) {
      GameState::Get().AddLogMessage("You can only plant on tilled land!");
      return;
   }

   auto item = currentLandmark->GetItem(x, y);
   if (item != nullptr) {
      GameState::Get().AddLogMessageFmt("Cannot plant, %s is on the ground here.", item->GetName().c_str());
      return;
   }

   auto crop = GameState::Get().GetItemFromItemDatabase(this->Crop);
   crop->SetCount(1);
   currentLandmark->AddItem(x, y, crop);

   auto growableInterface = crop->GetInterface<Growable>(ItemInterfaceType::Growable);
   if (growableInterface != nullptr) {
      growableInterface->StartGrowing(crop);
   }

   sourceItem->RemoveOne();

}

ItemInterfaceType::ItemInterfaceType Plantable::GetInterfaceType() {
   return ItemInterfaceType::Plantable;
}

void Plantable::Use(ItemPtr sourceItem) {
   this->Plant(sourceItem);
}

void Plantable::OnItemEquipped(ItemPtr sourceItem) {}

void Plantable::OnItemUnequipped(ItemPtr sourceItem) {}

