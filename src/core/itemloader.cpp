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

#include "itemloader.h"
#include "iteminterface.h"
#include <fstream>

std::list<Item> ItemLoader::LoadItemDatabase(std::string fileName)
{
   std::ifstream fileStream(fileName);
   picojson::value v;
   fileStream >> v;
   std::string err = picojson::get_last_error();
   if (!err.empty()) {
      std::cerr << err << std::endl;
      exit(1);
   }
   if (!v.is<picojson::object>()) {
      std::cerr << "JSON is not an object" << std::endl;
      exit(2);
   }
   const picojson::value::object& obj = v.get<picojson::object>();
   for (auto i : obj) {
      ItemLoader::ParseItemsDictionary(i);
   }

   return std::list<Item>();
}

void ItemLoader::ParseItemsDictionary(std::pair<const std::string, picojson::value> item)
{
   if (item.first != "items") {
      throw;
   }

   if (!item.second.is<picojson::object>()) {
      throw;
   }

   auto entries = item.second.get<picojson::object>();
   for (auto entry : entries) {
      ItemLoader::ParseItemTopLevel(entry);
   }

}

Item ItemLoader::ParseItemTopLevel(std::pair<const std::string, picojson::value> item)
{
   if (!item.second.is<picojson::object>()) {
      throw;
   }

   Item result;
   result.SetName(item.first);

   auto entries = item.second.get<picojson::object>();
   for (auto entry : entries) {
      ItemLoader::ParseItemTopLevelAttribute(&result, entry);
   }
   
   return result;
}

void ItemLoader::ParseItemTopLevelAttribute(Item * item, std::pair<const std::string, picojson::value> source)
{
   auto attributeName = source.first;

   if (attributeName == "description") {
      if (!source.second.is<std::string>()) {
         throw;
      }
      auto description = source.second.get<std::string>();
      item->SetDescription(description);
      return;
   }

   if (attributeName == "colorCode") {
      if (!source.second.is<std::string>()) {
         throw;
      }
      auto colorStr = source.second.get<std::string>();
      auto color = Color::FromString(colorStr);
      if (color == Color::Unknown || color != Color::Pure(color)) {
         throw;
      }
      item->SetColorCode(color);
      return;
   }

   if (attributeName == "characterCode") {
      if (!source.second.is<std::string>()) {
         throw;
      }
      
      auto characterCode = source.second.get<std::string>();
      if (characterCode.size() != 1) {
         throw;
      }

      item->SetCharacterCode(characterCode.at(0));
      return;
   }

   if (attributeName == "gfxTileCode") {
      if (!source.second.is<double>()) {
         throw;
      }

      auto gfxTileCode = source.second.get<double>();
      if (gfxTileCode != (unsigned int)gfxTileCode) {
         throw;
      }

      item->SetGfxTileCode((int)gfxTileCode);
      return;
   }

   if (attributeName == "surfaceAttributes") {
      if (!source.second.is<picojson::array>()) {
         throw;
      }
      ItemLoader::ParseSurfaceAttributes(item, source.second.get<picojson::array>());
      return;
   }

   if (attributeName == "itemCategories") {
      if (!source.second.is<picojson::array>()) {
         throw;
      }

      auto items = source.second.get<picojson::array>();
      std::list<ItemCategory::ItemCategory> itemCategories;
      for (auto item : items) {
         if (!item.is<std::string>()) {
            throw;
         }

         auto category = ItemCategory::FromString(item.get<std::string>());
         if (category == ItemCategory::Unknown) {
            throw;
         }

         itemCategories.push_back(category);
      }

      item->SetItemCategories(itemCategories);
      return;
   }

   if (attributeName == "interfaces") {
      if (!source.second.is<picojson::object>()) {
         throw;
      }

      ItemLoader::ParseItemInterfaces(item, source.second);
      return;
   }

   throw;
}

void ItemLoader::ParseItemInterfaces(Item * item, picojson::value source)
{
   for (auto iface : source.get<picojson::object>()) {
      auto interfaceType = ItemInterfaceType::FromString(iface.first);
      if (interfaceType == ItemInterfaceType::Unknown) {
         throw;
      }
      auto itemInterface = IItemInterface::Deserialize(interfaceType, iface.second);
      item->AddInterface(interfaceType, itemInterface);
   }
}

void ItemLoader::ParseSurfaceAttributes(Item * item, picojson::array surfaceAttributes)
{
   SurfaceAttribute::SurfaceAttribute result;

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

   item->SetSurfaceAttributes(result);
}
