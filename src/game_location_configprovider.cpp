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

#include "game_location_configprovider.h"
#include <fstream>

GameLocationConfigProvider::GameLocationConfigProvider() {
   std::ifstream configFile("settings.json");

   if (configFile) {
      // Deserialize config from file
      picojson::value configValue;
      configFile >> configValue;
      configFile.close();

      this->config = Config::Deserialize(configValue);
   } else {
      this->config = Config();
   }
}

Config GameLocationConfigProvider::GetConfig() {
   return this->config;
}

void GameLocationConfigProvider::SaveConfig() {
   // Serialize configuration to file
   std::ofstream configFile("settings.json");

   auto json = Config::Serialize(this->config);
   configFile << json.serialize(true);
   configFile.close();
}
