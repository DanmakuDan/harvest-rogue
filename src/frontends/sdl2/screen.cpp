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

#include "screen.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <sstream>

static int TileSize = 16;
static int Zoom = 1;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *fontTexture;
static SDL_Texture *tileTexture;
static int ActualTileSize = TileSize * Zoom;

static int TextTilesPerRow;
static int GfxTilesPerRow;

typedef struct {
   unsigned char r, g, b;
} sSdlColorMap;

static const sSdlColorMap SdlColors[] = {
   { 0x00, 0x00, 0x00 },  // CLR_BLACK    
   { 0xC0, 0x00, 0x00 },  // CLR_RED      
   { 0x00, 0xC0, 0x00 },  // CLR_GREEN    
   { 0xC0, 0xC0, 0x00 },  // CLR_YELLOW   
   { 0x00, 0x00, 0xC0 },  // CLR_BLUE     
   { 0xC0, 0x00, 0xC0 },  // CLR_MAGENTA  
   { 0x00, 0xC0, 0xC0 },  // CLR_CYAN     
   { 0xC0, 0xC0, 0xC0 },  // CLR_SILVER   
   { 0x80, 0x80, 0x80 },  // CLR_GRAY     
   { 0xFF, 0x00, 0x00 },  // CLR_BRRED    
   { 0x00, 0xFF, 0x00 },  // CLR_BRGREEN  
   { 0xFF, 0xFF, 0x00 },  // CLR_BRYELLOW 
   { 0x00, 0x00, 0xFF },  // CLR_BRBLUE   
   { 0xFF, 0x00, 0xFF },  // CLR_BRMAGENTA
   { 0x00, 0xFF, 0xFF },  // CLR_BRCYAN   
   { 0xFF, 0xFF, 0xFF }   // CLR_WHITE    
};


Screen::Screen() {
   SDL_Init(SDL_INIT_EVERYTHING);
   IMG_Init(IMG_INIT_PNG);
   Mix_Init(MIX_INIT_OGG);

   int windowWidth = Config::provider->GetConfig().GetScreenWidth();
   int windowHeight = Config::provider->GetConfig().GetScreenHeight();

   SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WindowFlags::SDL_WINDOW_RESIZABLE, &window, &renderer);
   SDL_SetWindowTitle(window, "Harvest-Rogue - Graphical Mode");

   int w, h;
   fontTexture = IMG_LoadTexture(renderer, "media/font16.png");
   SDL_QueryTexture(fontTexture, NULL, NULL, &w, &h);
   TextTilesPerRow = w / TileSize;

   tileTexture = IMG_LoadTexture(renderer, "media/tileset16.png");
   SDL_QueryTexture(tileTexture, NULL, NULL, &w, &h);
   GfxTilesPerRow = w / TileSize;

   SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Screen::~Screen()
{
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   Mix_Quit();
   IMG_Quit();
   SDL_Quit();
}

int Screen::GetWidth()
{
   int width, height;
   SDL_GetWindowSize(window, &width, &height);
   return width / ActualTileSize;
}

int Screen::GetHeight()
{
   int width, height;
   SDL_GetWindowSize(window, &width, &height);
   return height / ActualTileSize;
}

void Screen::WriteText(int x, int y, std::string text, Color::Color color)
{
   int startX = x;
   std::istringstream iss(text);
   for (std::string line; std::getline(iss, line);) {
      for (auto ch : line) {
         this->WriteCharacter(x, y, ch, color);
         x++;
      }
      x = startX;
      y++;
   }
}

void Screen::WriteCenterText(int y, std::string text, Color::Color color)
{
   auto strWidth = text.find('\n');
   int x = (this->GetWidth() / 2) - ((int)strWidth / 2);
   this->WriteText(x, y, text, color);
}

void Screen::WriteButton(int x, int y, int width, std::string text, bool active)
{
   auto captionLeft = x + (width / 2) - (text.length() / 2);

   if (true == active) {
      SDL_Rect destRect;
      destRect.x = x * TileSize * Zoom;
      destRect.y = y * TileSize * Zoom;
      destRect.h = TileSize * Zoom;
      destRect.w = TileSize * Zoom * width;
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderFillRect(renderer, &destRect);
   }

   this->WriteText((int)captionLeft, y, text, active ? Color::Inverse(Color::White) : Color::White);
}

void Screen::ClearLine(int y, Color::Color color)
{
   if (color == Color::Default) {
      color = Color::Black;
   }
   SDL_Rect destRect;
   destRect.x = 0;
   destRect.y = y * ActualTileSize;
   destRect.w = this->GetWidth() * ActualTileSize;
   destRect.h = ActualTileSize;

   if (color > 15) {
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
   }
   else {
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
   }
   SDL_RenderFillRect(renderer, &destRect);
}

void Screen::WriteCharacter(int x, int y, const char character, Color::Color color)
{
   if (color == Color::Default -1) {
      color = Color::Black;
   }
   SDL_Rect srcRect;
   SDL_Rect destRect;

   int tileY = character / TextTilesPerRow;
   int tileX = character % TextTilesPerRow;
   srcRect.x = tileX * TileSize;
   srcRect.y = tileY * TileSize;
   srcRect.w = srcRect.h = TileSize;

   destRect.x = x * TileSize * Zoom;
   destRect.y = y * TileSize * Zoom;
   destRect.w = destRect.h = TileSize * Zoom;

   if (color > 15) {
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderFillRect(renderer, &destRect);
   }
   //else {
   //   SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
   //}
   //SDL_RenderFillRect(renderer, &destRect);
   SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
   auto c = SdlColors[color % 16];
   if (color == 31) {
      SDL_SetTextureColorMod(fontTexture, ~c.r, ~c.g, ~c.b);
   }
   else {
      SDL_SetTextureColorMod(fontTexture, c.r, c.g, c.b);
   }
   SDL_RenderCopy(renderer, fontTexture, &srcRect, &destRect);
}

void Screen::WriteTile(int x, int y, int tileIndex, char character, Color::Color color)
{
   SDL_Rect srcRect;
   SDL_Rect destRect;
   int tileY = tileIndex / GfxTilesPerRow;
   int tileX = tileIndex % GfxTilesPerRow;
   srcRect.x = tileX * TileSize;
   srcRect.y = tileY * TileSize;
   srcRect.w = srcRect.h = TileSize;

   destRect.x = x * TileSize * Zoom;
   destRect.y = y * TileSize * Zoom;
   destRect.w = destRect.h = TileSize * Zoom;

   SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
   SDL_RenderCopy(renderer, tileTexture, &srcRect, &destRect);

}

void Screen::BeginScreenUpdate()
{
   SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
   SDL_RenderClear(renderer);
}

void Screen::EndScreenUpdate()
{
   SDL_RenderPresent(renderer);
}

void Screen::WriteWindow(int x, int y, int width, int height, std::string text)
{
   for (auto posY = y; posY < y + height; posY++) {
      for (auto posX = x; posX < x + width; posX++) {

         if (posY == y) {
            // Top row
            if (posX == x) {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowTopLeft).GfxTileCode, 0, Color::Default);
            }
            else if (posX == (x + width - 1)) {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowTopRight).GfxTileCode, 0, Color::Default);
            }
            else {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowTop).GfxTileCode, 0, Color::Default);
            }
         }
         else if (posY == y + (height - 1)) {
            // Bottom row
            if (posX == x) {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowBottomLeft).GfxTileCode, 0, Color::Default);
            }
            else if (posX == (x + width - 1)) {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowBottomRight).GfxTileCode, 0, Color::Default);
            }
            else {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowBottom).GfxTileCode, 0, Color::Default);
            }
         }
         else {
            // Middle Row
            if (posX == x) {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowLeft).GfxTileCode, 0, Color::Default);
            }
            else if (posX == (x + width - 1)) {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowRight).GfxTileCode, 0, Color::Default);
            }
            else {
               this->WriteTile(posX, posY, Tile::FromTileType(TileType::WindowCenter).GfxTileCode, 0, Color::Default);
            }
         }

         

      }
   }

   auto captionLeft = (x + (width / 2)) - (text.size() / 2);
   this->WriteText((int)captionLeft, y, text.c_str(), Color::White);
}
