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
#include <iostream>
#include <curses.h>

#include "screen.h"

Screen::Screen() {
   initscr();
#ifdef WIN32
   PDC_set_title("Harvest Rogue");
   resize_term(40, 120);
#endif
   noecho();
   cbreak();
   keypad(stdscr, TRUE);
   nodelay(stdscr, FALSE);
   notimeout(stdscr, TRUE);
   raw();
   curs_set(0);
   start_color();
   clear();

   for (short i = 0; i < 16; i++) {
      init_pair(i + 1, i, CLR_BLACK);
   }
   for (short i = 16; i < 32; i++) {
      init_pair(i + 1, i - 15, CLR_WHITE);
   }

}

Screen::~Screen() {
   endwin();
}

int Screen::GetWidth() {
   return getmaxx(stdscr);
}

int Screen::GetHeight() {
   return getmaxy(stdscr);
}

void Screen::WriteText(int x, int y, std::string text, int color) {

   std::istringstream iss(text);
   wattron(stdscr, COLOR_PAIR(color + 1));
   for (std::string line; std::getline(iss, line);) {
      mvwprintw(stdscr, y++, x, line.c_str());
   }
   wattroff(stdscr, COLOR_PAIR(color + 1));

}

void Screen::WriteCenterText(int y, std::string text, int color) {
   auto strWidth = text.find('\n');
   auto x = (this->GetWidth() / 2) - (strWidth / 2);
   this->WriteText(x, y, text, color);

}

void Screen::WriteButton(int x, int y, int width, std::string text, bool active) {
	wattron(stdscr, COLOR_PAIR(1 + CLR_PURE(CLR_WHITE)));
   if (active) {
      wattron(stdscr, A_BOLD | A_REVERSE);
   } else {
   }
   for (auto i = 0; i < width; i++) {
      mvwprintw(stdscr, y, x + i, " ");
   }
   auto captionLeft = x + (width / 2) - (text.length() / 2);
   this->WriteText(captionLeft, y, text, CLR_WHITE);
   if (active) {
	  wattroff(stdscr, A_BOLD | A_REVERSE);
   } else {
      wattroff(stdscr, COLOR_PAIR(1 + CLR_WHITE));
   }
   wattroff(stdscr, COLOR_PAIR(1 + CLR_PURE(CLR_WHITE)));
}

void Screen::ClearLine(int y, int color) {
   wmove(stdscr, y, 0);
   wattron(stdscr, COLOR_PAIR(color + 1));
   for (auto i = 0; i < this->GetWidth(); i++) {
      wprintw(stdscr, " ");
   }
   wattroff(stdscr, COLOR_PAIR(1));
}

void Screen::WriteCharacter(int x, int y, const char character, int color) {
   wattron(stdscr, COLOR_PAIR(1 + color));
   wmove(stdscr, y, x);
   waddch(stdscr, character);
   wattroff(stdscr, COLOR_PAIR(1 + color));
}

void Screen::WriteTile(int x, int y, Tile tile) {
   Screen::WriteCharacter(x, y, tile.CharacterCode, tile.ColorCode);
}

void Screen::BeginScreenUpdate() {
   erase();
}

void Screen::EndScreenUpdate() {

}

void Screen::WriteWindow(int x, int y, int width, int height, std::string text) {
   wattron(stdscr, COLOR_PAIR(1 + CLR_BRYELLOW));
   for(auto posY = y; posY < y + height; posY++) {
      for(auto posX = x; posX < x + width; posX++) {
         wmove(stdscr, posY, posX);
         if (posX == x || posX == x+width-1 || posY == y || posY == y+height-1) {
            waddch(stdscr, '=');
         } else {
            waddch(stdscr, ' ');
         }

      }
   }
   wattroff(stdscr, COLOR_PAIR(1 + CLR_YELLOW));
   wattron(stdscr, COLOR_PAIR(1 + CLR_WHITE));
   auto captionLeft = (x + (width / 2)) - (text.size() / 2);
   wattron(stdscr, A_BOLD);
   wmove(stdscr, y, captionLeft);
   wprintw(stdscr, text.c_str());
   wattroff(stdscr, A_BOLD);
   wattroff(stdscr, COLOR_PAIR(1 + CLR_WHITE));
}
