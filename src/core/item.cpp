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

std::map<ItemInterfaceType::ItemInterfaceType, std::shared_ptr<IItemInterface>> Item::GetInterfaces()
{
   return this->ItemInterfaces;
}

std::shared_ptr<IItemInterface> Item::GetInterface(ItemInterfaceType::ItemInterfaceType itemInterfaceType)
{
   return this->ItemInterfaces[itemInterfaceType];
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
   this->SurfaceAttributes == surfaceAttributes;
}

void Item::SetColorCode(int colorCode)
{
   this->ColorCode = colorCode;
}

int Item::GetColorCode()
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
