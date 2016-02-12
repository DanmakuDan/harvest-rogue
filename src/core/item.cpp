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

#include "item.h"
#include "gamestate.h"
#include "choppingtool.h"
#include "tillingtool.h"
#include "durable.h"
#include "dropsloot.h"


Item Item::Clone(const Item & source)
{
   Item result = source;

   for (auto i : source.ItemInterfaces) {
      if (i.second == nullptr) {
         result.ItemInterfaces[i.first] = nullptr;
         continue;
      }
      result.ItemInterfaces[i.first] = std::shared_ptr<IItemInterface>(i.second->Clone());
   }

   return result;
}

Item::Item()
{
   this->Count = 1;
}

std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> Item::GetInterfaces()
{
   return this->ItemInterfaces;
}

bool Item::HasInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType)
{
   return this->ItemInterfaces[itemInterfaceType] != nullptr;
}

void Item::AddInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType, std::shared_ptr<IItemInterface> itemInterface)
{
   if (this->ItemInterfaces[itemInterfaceType] != nullptr) {
      throw;
   }

   this->ItemInterfaces[itemInterfaceType] = std::shared_ptr<IItemInterface>(itemInterface);
}

void Item::RemoveInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType)
{
   this->ItemInterfaces.erase(itemInterfaceType);
}

std::string Item::GetName()
{
   return this->Name;
}

void Item::SetName(std::string name)
{
   this->Name = name;
}

std::string Item::GetDescription()
{
   return this->Description;
}

bool Item::Takeable()
{
   return this->HasInterface(ItemInterfaceType::Obtainable);
}

bool Item::IsUsable()
{
   if (this->HasInterface(ItemInterfaceType::Durable)) {
      auto durableInterface = this->GetInterface<Durable>(ItemInterfaceType::Durable);
      if (durableInterface->GetDurability() <= 0) {
         GameState::Get().AddLogMessageFmt("The %s is broken and cannot be used.", this->GetName().c_str());
         return false;
      }
   }

   if (this->HasInterface(ItemInterfaceType::ChoppingTool)
      || this->HasInterface(ItemInterfaceType::TillingTool)
      )
      return true;
   return false;
}

bool Item::IsEquippable()
{
   if (this->HasInterface(ItemInterfaceType::ChoppingTool)
      || this->HasInterface(ItemInterfaceType::TillingTool)
      )
      return true;
   return false;
}

void Item::Use()
{
   if (this->HasInterface(ItemInterfaceType::ChoppingTool)) {
      auto choppingTool = this->GetInterface<ChoppingTool>(ItemInterfaceType::ChoppingTool);
      choppingTool->Chop(this->shared_from_this());
      return;
   }   
}

void Item::Use(Direction::Direction direction)
{
   if (this->HasInterface(ItemInterfaceType::ChoppingTool)) {
      auto choppingTool = this->GetInterface<ChoppingTool>(ItemInterfaceType::ChoppingTool);
      choppingTool->Chop(this->shared_from_this(), direction);
      return;
   }
}

void Item::Destruct()
{
   auto currentLandmark = GameState::Get().GetCurrentLandmark();
   int x, y;
   currentLandmark->LocateItem(this->shared_from_this(), x, y);
   currentLandmark->RemoveItem(x, y);

   if (this->HasInterface(ItemInterfaceType::DropsLoot)) {
      auto dropsLootInterface = this->GetInterface<DropsLoot>(ItemInterfaceType::DropsLoot);
      dropsLootInterface->DropLoot(x, y);
   }

   GameState::Get().AddLogMessageFmt("The %s was destroyed!", this->GetName().c_str());
   
}

void Item::SetDescription(std::string description)
{
   this->Description = description;
}

SurfaceAttribute::SurfaceAttribute Item::GetSurfaceAttributes()
{
   return this->SurfaceAttributes;
}

void Item::SetSurfaceAttributes(SurfaceAttribute::SurfaceAttribute surfaceAttributes)
{
   this->SurfaceAttributes = surfaceAttributes;
}

void Item::SetColorCode(Color::Color colorCode)
{
   this->ColorCode = colorCode;
}

Color::Color Item::GetColorCode()
{
   return this->ColorCode;
}

void Item::SetCharacterCode(char characterCode)
{
   this->CharacterCode = characterCode;
}

char Item::GetCharacterCode()
{
   return this->CharacterCode;
}

void Item::SetGfxTileCode(int gfxTileCode)
{
   this->GfxTileCode = gfxTileCode;
}

int Item::GetGfxTileCode()
{
   return this->GfxTileCode;
}

std::list<ItemCategory::ItemCategory> Item::GetItemCategories()
{
   return this->ItemCategories;
}

void Item::SetItemCategories(std::list<ItemCategory::ItemCategory> itemCategories)
{
   this->ItemCategories = itemCategories;
}

int Item::GetCount()
{
   return this->Count;
}

void Item::SetCount(int count)
{
   this->Count = count;
}
