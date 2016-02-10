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

file(GLOB SOURCE_FILES "${harvest_rogue_SOURCE_DIR}/src/*.cpp")

file(GLOB SOURCE_SCENE_FILES "${harvest_rogue_SOURCE_DIR}/src/scenes/*.cpp")
   source_group(scenes FILES ${SOURCE_SCENE_FILES})

file(GLOB SOURCE_DIALOG_FILES "${harvest_rogue_SOURCE_DIR}/src/dialogs/*.cpp")
   source_group(dialogs FILES ${SOURCE_DIALOG_FILES})

file(GLOB SOURCE_PROP_FILES "${harvest_rogue_SOURCE_DIR}/src/props/*.cpp")
   source_group(props FILES ${SOURCE_PROP_FILES})

file(GLOB SOURCE_NCURSES_FILES "${harvest_rogue_SOURCE_DIR}/src/frontends/ncurses/*.cpp")
   source_group(ncurses FILES ${SOURCE_NCURSES_FILES})

file(GLOB SOURCE_CORE_FILES "${harvest_rogue_SOURCE_DIR}/src/core/*.cpp")
source_group(core FILES ${SOURCE_CORE_FILES})

file(GLOB SOURCE_COMMON_FILES "${harvest_rogue_SOURCE_DIR}/src/common/*.cpp")
source_group(common FILES ${SOURCE_COMMON_FILES})

file(GLOB HEADER_FILES
   "${harvest_rogue_SOURCE_DIR}/include/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/common/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/dialogs/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/interfaces/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/props/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/scenes/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/core/*.h"
)
   source_group(include FILES ${HEADER_FILES})

include_directories(
   ${harvest_rogue_SOURCE_DIR}/include
   ${harvest_rogue_SOURCE_DIR}/include/common
   ${harvest_rogue_SOURCE_DIR}/include/dialogs
   ${harvest_rogue_SOURCE_DIR}/include/interfaces
   ${harvest_rogue_SOURCE_DIR}/include/props
   ${harvest_rogue_SOURCE_DIR}/include/scenes
   ${harvest_rogue_SOURCE_DIR}/include/core
)

IF(WIN32)
   # We must manually include PDCurses to emulate NCurses
   include_directories("${harvest_rogue_SOURCE_DIR}/windows/pdcurses")
   include_directories("${harvest_rogue_SOURCE_DIR}/windows/pdcurses/win32")

   file(GLOB SOURCE_WIN32_PDCURSES_FILES
      "${harvest_rogue_SOURCE_DIR}/windows/pdcurses/pdcurses/*.c"
	  "${harvest_rogue_SOURCE_DIR}/windows/pdcurses/win32/*.c"
   )
   source_group(pdcurses FILES ${SOURCE_WIN32_PDCURSES_FILES})
ELSE()
   find_package(Curses REQUIRED)
ENDIF()
		
add_executable(harvest_rogue
   ${SOURCE_FILES}
   ${SOURCE_SCENE_FILES}
   ${SOURCE_DIALOG_FILES}
   ${SOURCE_PROP_FILES}
   ${SOURCE_NCURSES_FILES}
   ${SOURCE_CORE_FILES}
   ${SOURCE_COMMON_FILES}
   ${HEADER_FILES}
   ${SOURCE_WIN32_PDCURSES_FILES}
)

target_link_libraries(harvest_rogue ${CURSES_LIBRARIES})

IF(WIN32)
set_target_properties(harvest_rogue PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
ENDIF()