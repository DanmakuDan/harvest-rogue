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

#include "config.h"

int Config::GetScreenWidth() {
   return this->screenWidth;
}

void Config::SetScreenWidth(int width) {
   this->screenWidth = width;
}

int Config::GetScreenHeight() {
   return this->screenHeight;
}

void Config::SetScreenHeight(int height) {
   this->screenHeight = height;
}

Keybinding Config::GetKeybinding() {
   return this->keybinding;
}

void Config::SetKeybinding(Keybinding binding) {
   this->keybinding = binding;
}

picojson::value Config::Serialize(Config config) {
   picojson::object result;

   result["screenWidth"] = picojson::value((double)config.GetScreenWidth());
   result["screenHeight"] = picojson::value((double)config.GetScreenHeight());

   // TODO: Include keybindings?

   return picojson::value(result);
}

Config Config::Deserialize(picojson::value serializedValue) {
   auto result = Config();

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {
      if (item.first == "screenWidth") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         result.SetScreenWidth(value);
         continue;
      }

      if (item.first == "screenHeight") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         result.SetScreenHeight(value);
         continue;
      }

      if (item.first == "keybindings") {
         result.SetKeybinding(Keybinding::Deserialize(item.second));
         continue;
      }

      throw;
   }

   return result;
}
