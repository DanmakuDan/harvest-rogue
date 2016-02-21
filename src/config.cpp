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
   return this->ScreenWidth;
}

void Config::SetScreenWidth(int width) {
   this->ScreenWidth = width;
}

int Config::GetScreenHeight() {
   return this->ScreenHeight;
}

void Config::SetScreenHeight(int height) {
   this->ScreenHeight = height;
}

Keybinding Config::GetKeybinding() {
   return this->keybinding;
}

void Config::SetKeybinding(Keybinding binding) {
   this->keybinding = binding;
}

picojson::value Config::Serialize() {
   picojson::object result;

   result["screenWidth"] = picojson::value((double)this->GetScreenWidth());
   result["screenHeight"] = picojson::value((double)this->GetScreenHeight());
   result["screenX"] = picojson::value((double)this->GetScreenX());
   result["screenY"] = picojson::value((double)this->GetScreenY());
   // TODO: Include keybindings?

   return picojson::value(result);
}

void Config::Deserialize(picojson::value serializedValue) {
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
         if (value == 0) {
            continue;
         }
         this->SetScreenWidth((int)value);
         continue;
      }

      if (item.first == "screenHeight") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value == 0) {
            continue;
         }
         this->SetScreenHeight((int)value);
         continue;
      }

      if (item.first == "screenX") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value == 0) {
            continue;
         }
         this->SetScreenX((int)value);
         continue;
      }

      if (item.first == "screenY") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();
         if (value == 0) {
            continue;
         }
         this->SetScreenY((int)value);
         continue;
      }

      if (item.first == "keybindings") {
         this->SetKeybinding(Keybinding::Deserialize(item.second));
         continue;
      }

      throw;
   }
}

int Config::GetScreenX() {
   return this->ScreenX;
}

void Config::SetScreenX(int x) {
   this->ScreenX = x;
}

int Config::GetScreenY() {
   return this->ScreenY;
}

void Config::SetScreenY(int y) {
   this->ScreenY = y;
}
