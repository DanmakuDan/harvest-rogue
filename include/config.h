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

#ifndef HARVEST_ROGUE_CONFIG_H
#define HARVEST_ROGUE_CONFIG_H

#include "keybinding.h"
#include "picojson.h"
#include <memory>
#include <fstream>

class IConfigProvider;
class Config;
typedef std::shared_ptr<Config> ConfigPtr;
class Config {
private:
   Config(Config const &) { };
   Config &operator=(Config const &) { };

public:
   static Config &Get() {
      static Config config;
      return config;
   }

   int GetScreenWidth() const;
   void SetScreenWidth(int width);

   int GetScreenHeight() const;
   void SetScreenHeight(int height);

   int GetScreenX() const;
   void SetScreenX(int x);

   int GetScreenY() const;
   void SetScreenY(int y);

   Keybinding GetKeybinding() const;
   void SetKeybinding(Keybinding binding);

private:
   Config() {
      std::ifstream configFile("settings.json");

      if (!configFile) {
         // Accept defaults
         return;
      }

      // Deserialize config from file
      picojson::value configValue;
      configFile >> configValue;
      configFile.close();

      this->Deserialize(configValue);
   }
   ~Config() {
      std::ofstream configFile("settings.json");

      auto json = this->Serialize();
      configFile << json.serialize(true);
      configFile.close();
   }
   picojson::value Serialize();
   void Deserialize(picojson::value serializedValue);
   int ScreenWidth = 1280;
   int ScreenHeight = 720;
   int ScreenX = 30;
   int ScreenY = 50;

   Keybinding keybinding = Keybinding();
};

#endif // HARVEST_ROGUE_CONFIG_H
