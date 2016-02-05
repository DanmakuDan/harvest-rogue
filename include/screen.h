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

#ifndef HARVEST_ROGUE_SCREEN_H
#define HARVEST_ROGUE_SCREEN_H

#include <memory>
#include <string>

#define CLR_DEFAULT    -1
#define CLR_BLACK       0
#define CLR_RED         1
#define CLR_GREEN       2
#define CLR_YELLOW      3
#define CLR_BLUE        4
#define CLR_MAGENTA     5
#define CLR_CYAN        6
#define CLR_SILVER      7
#define CLR_GRAY        8
#define CLR_BRRED       9
#define CLR_BRGREEN     10
#define CLR_BRYELLOW    11
#define CLR_BRBLUE      12
#define CLR_BRMAGENTA   13
#define CLR_BRCYAN      14
#define CLR_WHITE       15
#define CLR_INVERSE(X) (X+16)

class Screen {
private:
   Screen();

   Screen(Screen const &) { };

   Screen &operator=(Screen const &) { };
public:
   ~Screen();

   static Screen &Get() {
      static Screen screen;
      return screen;
   }

   int GetWidth();

   int GetHeight();

   void Clear();

   void WriteText(int x, int y, std::string text, int color = CLR_WHITE);

   void WriteCenterText(int y, std::string text, int color = CLR_WHITE);

   void WriteButton(int x, int y, int width, std::string text, bool active);

   void ClearLine(int y, int color = CLR_WHITE);
};


#endif //HARVEST_ROGUE_SCREEN_H

