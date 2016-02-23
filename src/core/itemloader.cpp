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
#include <fstream>

std::map<std::string, ItemPtr> ItemLoader::LoadItemDatabase(std::string fileName)
{
   std::map<std::string, ItemPtr> result;

   std::ifstream fileListStream(fileName);
   picojson::value fileListValue;
   fileListStream >> fileListValue;
   auto definitionFilesObject = fileListValue.get<picojson::object>();
   for (auto definitionFilesObjectItem : definitionFilesObject) {
      if (definitionFilesObjectItem.first != "definitionFiles") {
         throw;
      }

      auto defFileItems = definitionFilesObjectItem.second.get<picojson::array>();
      for (auto definitionFileName : defFileItems) {
         auto fn = definitionFileName.get<std::string>();

         std::ifstream fileStream(fn);
         picojson::value v;
         fileStream >> v;

         auto obj = v.get<picojson::object>();
         for (auto i : obj) {
            if (i.first != "items") {
               continue;
            }

            if (!i.second.is<picojson::object>()) {
               throw;
            }

            auto entries = i.second.get<picojson::object>();
            for (auto entry : entries) {
               auto item = ItemLoader::ParseItemTopLevel(entry);
               result[item->GetName()] = item;
            }

            break;
         }
      }
   }
   return result;
}

ItemPtr ItemLoader::ParseItemTopLevel(std::pair<const std::string, picojson::value> item)
{
   if (!item.second.is<picojson::object>()) {
      throw;
   }

   ItemPtr result = ItemPtr(new Item());
   result->SetName(item.first);

   auto entries = item.second.get<picojson::object>();
   for (auto entry : entries) {
      ItemLoader::ParseItemTopLevelAttribute(result, entry);
   }

   return std::shared_ptr<Item>(result);
}

void ItemLoader::ParseItemTopLevelAttribute(ItemPtr item, std::pair<const std::string, picojson::value> source)
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
      for (auto curItem : items) {
         if (!curItem.is<std::string>()) {
            throw;
         }

         auto category = ItemCategory::FromString(curItem.get<std::string>());
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

void ItemLoader::ParseItemInterfaces(ItemPtr item, picojson::value source)
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

void ItemLoader::ParseSurfaceAttributes(ItemPtr item, picojson::array surfaceAttributes)
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

   item->SetSurfaceAttributes(result);
}
