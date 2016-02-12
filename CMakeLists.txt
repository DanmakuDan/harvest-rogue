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

INCLUDE(FindPkgConfig)

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            file(COPY ${srcTemplatePath} DESTINATION ${destDir}/)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)

OPTION(BUILD_SDL2   "Build the SDL2 frontend." OFF)

configure_files(${harvest_rogue_SOURCE_DIR}/media/definitions ${CMAKE_BINARY_DIR}/media)
configure_files(${harvest_rogue_SOURCE_DIR}/media/definitions/system ${CMAKE_BINARY_DIR}/media)
   
IF(BUILD_SDL2)
   ADD_DEFINITIONS(-DGRAPHICS_MODE)
   file(GLOB SOURCE_FRONTEND_FILES "${harvest_rogue_SOURCE_DIR}/src/frontends/sdl2/*.cpp")
   source_group(sdl2 FILES ${SOURCE_FRONTEND_FILES})
   
   configure_files(${harvest_rogue_SOURCE_DIR}/media ${CMAKE_BINARY_DIR}/media)

   IF(WIN32)
      include_directories("${harvest_rogue_SOURCE_DIR}/windows/sdl2")
      set(SDL2_LIBRARIES SDL2 SDL2_image SDL2_mixer timidity)
      link_directories("${harvest_rogue_SOURCE_DIR}/windows/sdl2/x64")
     configure_files(${harvest_rogue_SOURCE_DIR}/windows/sdl2/x64/runtime ${CMAKE_BINARY_DIR})
   ELSE(WIN32)
      PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
      PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
      PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

      include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})
      IF(APPLE)
         include_directories("/usr/local/include")
         link_directories("/usr/local/lib")
      ENDIF(APPLE)

   ENDIF(WIN32)

ELSE(BUILD_SDL2)
   ADD_DEFINITIONS(-DTEXT_MODE)
   file(GLOB SOURCE_FRONTEND_FILES "${harvest_rogue_SOURCE_DIR}/src/frontends/ncurses/*.cpp")
   source_group(ncurses FILES ${SOURCE_FRONTEND_FILES})

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
ENDIF(BUILD_SDL2)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCE_FILES "${harvest_rogue_SOURCE_DIR}/src/*.cpp")
   source_group("" FILES ${SOURCE_FILES})

file(GLOB SOURCE_SCENE_FILES "${harvest_rogue_SOURCE_DIR}/src/scenes/*.cpp")
   source_group("Scenes" FILES ${SOURCE_SCENE_FILES})

file(GLOB SOURCE_DIALOG_FILES "${harvest_rogue_SOURCE_DIR}/src/dialogs/*.cpp")
   source_group("Dialogs" FILES ${SOURCE_DIALOG_FILES})

file(GLOB SOURCE_PROP_FILES "${harvest_rogue_SOURCE_DIR}/src/props/*.cpp")
   source_group("Props" FILES ${SOURCE_PROP_FILES})

file(GLOB SOURCE_CORE_FILES "${harvest_rogue_SOURCE_DIR}/src/core/*.cpp")
   source_group("Core" FILES ${SOURCE_CORE_FILES})

file(GLOB SOURCE_COMMON_FILES "${harvest_rogue_SOURCE_DIR}/src/common/*.cpp")
   source_group("Common" FILES ${SOURCE_COMMON_FILES})

file(GLOB SOURCE_ITEMINTERFACES_FILES "${harvest_rogue_SOURCE_DIR}/src/iteminterfaces/*.cpp")
   source_group("Item Interfaces" FILES ${SOURCE_ITEMINTERFACES_FILES})
   
file(GLOB HEADER_FILES
   "${harvest_rogue_SOURCE_DIR}/include/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/common/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/dialogs/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/interfaces/*.h"
   "${harvest_rogue_SOURCE_DIR}/include/iteminterfaces/*.h"
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
   ${harvest_rogue_SOURCE_DIR}/include/iteminterfaces
   ${harvest_rogue_SOURCE_DIR}/include/props
   ${harvest_rogue_SOURCE_DIR}/include/scenes
   ${harvest_rogue_SOURCE_DIR}/include/core
)

add_executable(harvest_rogue
   ${SOURCE_FILES}
   ${SOURCE_SCENE_FILES}
   ${SOURCE_DIALOG_FILES}
   ${SOURCE_PROP_FILES}
   ${SOURCE_FRONTEND_FILES}
   ${SOURCE_CORE_FILES}
   ${SOURCE_COMMON_FILES}
   ${SOURCE_ITEMINTERFACES_FILES}
   ${HEADER_FILES}
   ${SOURCE_WIN32_PDCURSES_FILES}
)

target_link_libraries(harvest_rogue 
   ${CURSES_LIBRARIES}
   ${SDL2_LIBRARIES}
   ${SDL2IMAGE_LIBRARIES}
   ${SDL2MIXER_LIBRARIES}
)

IF(WIN32)
   set_target_properties(harvest_rogue PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
ENDIF()

