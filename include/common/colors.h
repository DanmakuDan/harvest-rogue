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

#ifndef HARVEST_ROGUE_COLORS_H
#define HARVEST_ROGUE_COLORS_H

#include <string>

namespace Color {
   enum Color {
      Unknown = ~0,
      Default = -1,
      Black = 0,
      Red,
      Green,
      Yellow,
      Blue,
      Magenta,
      Cyan,
      Silver,
      Gray,
      BrightRed,
      BrightGreen,
      BrightYellow,
      BrightBlue,
      BrightMagenta,
      BrightCyan,
      White
   };

   static Color Inverse(Color color) {
      return (Color)(color + 16);
   }

   static Color Pure(Color color) {
      return (Color)((int)color & 0xF);
   }

   static Color FromString(std::string color) {
      if (color == "default") return Default;
      if (color == "black") return Black;
      if (color == "red") return Red;
      if (color == "green") return Green;
      if (color == "yellow") return Yellow;
      if (color == "blue") return Blue;
      if (color == "magenta") return Magenta;
      if (color == "cyan") return Cyan;
      if (color == "silver") return Silver;
      if (color == "gray") return Gray;
      if (color == "brightRed") return BrightRed;
      if (color == "brightGreen") return BrightGreen;
      if (color == "brightYellow") return BrightYellow;
      if (color == "brightBlue") return BrightBlue;
      if (color == "brightMagenta") return BrightMagenta;
      if (color == "brightCyan") return BrightCyan;
      if (color == "white") return White;
      return Unknown;
   }

}



#endif //HARVEST_ROGUE_COLORS_H
