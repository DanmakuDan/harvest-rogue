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


#ifndef HARVEST_ROGUE_ITEMLOADER_H
#define HARVEST_ROGUE_ITEMLOADER_H

#include <item.h>
#include <map>
#include <string>
#include "picojson.h"

class ItemLoader {
public:
   static std::map<std::string, ItemPtr> LoadItemDatabase(std::string fileName);
private:
   static ItemPtr ParseItemTopLevel(std::pair<const std::string, picojson::value> item);
   static void ParseItemTopLevelAttribute(ItemPtr item, std::pair<const std::string, picojson::value> source);
   static void ParseItemInterfaces(ItemPtr item, picojson::value source);
   static void ParseSurfaceAttributes(ItemPtr item, picojson::array surfaceAttributes);

};

#endif //HARVEST_ROGUE_ITEMLOADER_H
