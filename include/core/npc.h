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

#ifndef HARVEST_ROGUE_NPC_H
#define HARVEST_ROGUE_NPC_H

#include "nameable.h"
#include "colors.h"
#include <memory>
#include <string>

class NPC;
typedef std::shared_ptr<NPC> NPCPtr;
class NPC : public INameable,
   public std::enable_shared_from_this<NPC> {
public:
   NPC();
   std::string GetDescription() const;
   char GetCharacterCode() const;
   int GetGfxTileCode() const;
   Color::Color GetColorCode() const;

   // INameable
   std::string GetName() override;

private:
   std::string Name;
   std::string Description;
   Color::Color ColorCode;
   char CharacterCode;
   int GfxTileCode;
};


#endif // HARVEST_ROGUE_NPC_H
