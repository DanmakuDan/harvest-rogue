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

#ifndef HARVEST_ROGUE_CROPS_H
#define HARVEST_ROGUE_CROPS_H

#include <string>
#include <vector>
#include <map>
#include "crops.h"
#include "tiles.h"


namespace CropGrowthType {
   enum CropGrowthType {
      Seedling = 0,
      Growing,
      FullyGrown,
      Wilted
   };

   static std::string GetName(CropGrowthType cropGrowthType) {
      switch(cropGrowthType) {
         case Seedling:   return "Seedling";
         case Growing:    return "Growing";
         case FullyGrown: return "Fully grown";
         case Wilted:     return "Wilted";
      }
   };
}

namespace CropType {
   enum CropType {
      Potato,
      Wheat
   };
}


namespace Crop {
   typedef struct {
      CropType::CropType CropType;
      std::map<CropGrowthType::CropGrowthType, TileType::TileType> TileTypes;
      std::string Name;
      int HoursToGrow;
      int YieldPerHarvest;
   } Crop;


   static std::vector<Crop> Crops = {

         {CropType::Potato, {{CropGrowthType::Seedling,        TileType::PotatoSeedling   },
                             {CropGrowthType::Growing,         TileType::PotatoGrowing    },
                             {CropGrowthType::FullyGrown,      TileType::PotatoFullyGrown },
                             {CropGrowthType::Wilted,          TileType::PotatoWilted     }
                            }, "Potato", 24*3,  10},

         {CropType::Wheat, {{CropGrowthType::Seedling,         TileType::WheatSeedling    },
                            {CropGrowthType::Growing,          TileType::WheatGrowing     },
                            {CropGrowthType::FullyGrown,       TileType::WheatFullyGrown  },
                            {CropGrowthType::Wilted,           TileType::WheatWilted      }
                           }, "Wheat", (24*5) + 8,  3}
   };

   static Crop FromCropType(CropType::CropType cropType) {
      for (auto crop : Crops) {
         if (crop.CropType == cropType) {
            return crop;
         }
      }
   }

}

#endif //HARVEST_ROGUE_CROPS_H
