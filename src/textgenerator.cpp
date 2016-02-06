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

#include <sstream>
#include <random>
#include "textgenerator.h"
#include "adjectives.h"
#include "animalnouns.h"

static std::random_device randomDevice;
static std::default_random_engine generator(randomDevice());

std::string TextGenerator::GenerateFarmName() {
   std::uniform_int_distribution<int> adjectiveDistribution(0, ADJECTIVES_MAX);
   std::uniform_int_distribution<int> animalNounDistribution(0, ANIMALNOUNS_MAX);
   std::stringstream farmName;
   farmName << "The \"";
   auto adjective = Adjectives[adjectiveDistribution(generator)];
   adjective[0] = toupper(adjective[0]);
   farmName << adjective;
   farmName << " ";
   farmName << AnimalNouns[animalNounDistribution(generator)];
   farmName << "\" Farm";
   return farmName.str();
}
