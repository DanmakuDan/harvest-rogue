///*
//    harvest-rogue is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    harvest-rogue is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with harvest-rogue.  If not, see <http://www.gnu.org/licenses/>.     */
//
//#include "bed.h"
//#include "gamestate.h"
//
//Bed::Bed() {
//
//}
//
//std::string Bed::GetName() {
//   return "Bed";
//}
//
//std::string Bed::GetDescription() {
//   return "A warm bed";
//}
//
//TileType::TileType Bed::GetTileType() {
//   return TileType::Bed;
//}
//
//bool Bed::Takeable() {
//   return false;
//}
//
//void Bed::Interact() {
//   GameState::Get().AddLogMessage("You lay in the bed and go to sleep...");
//   GameState::Get().SleepUntilNextMorning();
//}
