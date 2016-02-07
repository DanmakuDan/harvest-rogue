# harvest-rogue is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# harvest-rogue is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with harvest-rogue.  If not, see <http://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 2.8.7)
project(harvest_rogue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR} "./include")

file(GLOB SOURCE_FILES
        "./src/*.cpp"
        "./src/scenes/*.cpp"
        "./src/dialogs/*.cpp"
        "./src/frontends/ncurses/*.cpp"
        "./include/*.h"
        )
add_executable(harvest_rogue ${SOURCE_FILES} src/frontends/ncurses/input.cpp include/input.h include/scene.h src/scenes/mainmenu.cpp include/mainmenu.h src/gamestate.cpp include/gamestate.h src/scenes/game.cpp include/game.h src/landmark.cpp include/landmark.h src/textgenerator.cpp include/textgenerator.h include/adjectives.h include/animalnouns.h include/tiles.h src/landmarkgenerator.cpp include/landmarkgenerator.h include/dialog.h src/dialogs/gamemenudialog.cpp include/gamemenudialog.h include/colors.h src/adjectives.cpp src/animalnouns.cpp)
target_link_libraries(harvest_rogue ${CURSES_LIBRARIES})
