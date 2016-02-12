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

#include "tillingtool.h"

TillingTool::TillingTool()
{
}

TillingTool::~TillingTool()
{
}

std::shared_ptr<TillingTool> TillingTool::Deserialize(picojson::value serializedValue)
{
   return std::shared_ptr<TillingTool>();
}

int TillingTool::GetStrength()
{
   return this->Strength;
}

void TillingTool::SetStrength(int strength)
{
   this->Strength = strength;
}

int TillingTool::GetFatigue()
{
   return this->Fatigue;
}

void TillingTool::SetFatigue(int fatigue)
{
   this->Fatigue = fatigue;
}

ItemInterfaceType::ItemInterfaceType TillingTool::GetInterfaceType()
{
   return ItemInterfaceType::ChoppingTool;
}
