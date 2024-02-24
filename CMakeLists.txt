cmake_minimum_required(VERSION 3.18)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum macOS version")

# Universal build for macOS
if(CMAKE_GENERATOR STREQUAL "Xcode")
  set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)" CACHE STRING "Target architectures for macOS")
else()
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Target architectures for macOS")
endif()

set(GAME_PATH "${CMAKE_CURRENT_SOURCE_DIR}/games/default-game" CACHE STRING "Path to the game to bundle")

project(SMGF
  VERSION 1.0.0
  LANGUAGES C
  DESCRIPTION "A lightweight, portable and open-source 2D game engine"
  HOMEPAGE_URL "https://github.com/superzazu/smgf")

message(STATUS
  "Configuring SMGF ${SMGF_VERSION} for ${CMAKE_SYSTEM_NAME} "
  "(generator: ${CMAKE_GENERATOR}, cross-compiling: ${CMAKE_CROSSCOMPILING}, CMake version: ${CMAKE_VERSION})")
message(STATUS "Bundling game ${GAME_PATH}")

# SETUP DEPENDENCIES

# # SDL2
if(EMSCRIPTEN)
  # we tell emscripten we want a html file alongside the javascript file:
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${GAME_PATH}@/game")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s MAXIMUM_MEMORY=1gb")
else()
  find_package(SDL2 REQUIRED)

  message(STATUS "SDL2 lib: ${SDL2_LIBRARIES}")
  message(STATUS "SDL2 inc: ${SDL2_INCLUDE_DIRS}")
endif()

# # Lua
set(LUA_SRCS
  deps/lua-5.4.6/src/lapi.c
  deps/lua-5.4.6/src/lcode.c
  deps/lua-5.4.6/src/lctype.c
  deps/lua-5.4.6/src/ldebug.c
  deps/lua-5.4.6/src/ldo.c
  deps/lua-5.4.6/src/ldump.c
  deps/lua-5.4.6/src/lfunc.c
  deps/lua-5.4.6/src/lgc.c
  deps/lua-5.4.6/src/llex.c
  deps/lua-5.4.6/src/lmem.c
  deps/lua-5.4.6/src/lobject.c
  deps/lua-5.4.6/src/lopcodes.c
  deps/lua-5.4.6/src/lparser.c
  deps/lua-5.4.6/src/lstate.c
  deps/lua-5.4.6/src/lstring.c
  deps/lua-5.4.6/src/ltable.c
  deps/lua-5.4.6/src/ltm.c
  deps/lua-5.4.6/src/lundump.c
  deps/lua-5.4.6/src/lvm.c
  deps/lua-5.4.6/src/lzio.c
  deps/lua-5.4.6/src/lauxlib.c
  deps/lua-5.4.6/src/lbaselib.c
  deps/lua-5.4.6/src/lcorolib.c
  deps/lua-5.4.6/src/ldblib.c
  deps/lua-5.4.6/src/liolib.c
  deps/lua-5.4.6/src/lmathlib.c
  deps/lua-5.4.6/src/loslib.c
  deps/lua-5.4.6/src/lstrlib.c
  deps/lua-5.4.6/src/ltablib.c
  deps/lua-5.4.6/src/lutf8lib.c
  deps/lua-5.4.6/src/loadlib.c
  deps/lua-5.4.6/src/linit.c
)
add_library(lua STATIC ${LUA_SRCS})

# if(NOT WIN32)
# target_link_libraries(lua PUBLIC m)
# endif()
target_include_directories(lua PRIVATE deps/lua-5.4.6/src)

# # physfs
option(PHYSFS_ARCHIVE_ZIP "" ON)
option(PHYSFS_ARCHIVE_7Z "" OFF)
option(PHYSFS_ARCHIVE_GRP "" OFF)
option(PHYSFS_ARCHIVE_WAD "" OFF)
option(PHYSFS_ARCHIVE_HOG "" OFF)
option(PHYSFS_ARCHIVE_MVL "" OFF)
option(PHYSFS_ARCHIVE_QPAK "" OFF)
option(PHYSFS_ARCHIVE_SLB "" OFF)
option(PHYSFS_ARCHIVE_VDF "" OFF)
option(PHYSFS_ARCHIVE_ISO9660 "" OFF)
option(PHYSFS_BUILD_DOCS "" OFF)
option(PHYSFS_BUILD_STATIC "" ON)
option(PHYSFS_BUILD_SHARED "" OFF)
option(PHYSFS_BUILD_TEST "" OFF)
set(PHYSFS_TARGETNAME_DOCS "physfs-docs")
set(PHYSFS_TARGETNAME_DIST "physfs-dist")
set(PHYSFS_TARGETNAME_UNINSTALL "physfs-uninstall")
add_subdirectory(deps/physfs EXCLUDE_FROM_ALL)

# # SDL_sound
option(SDLSOUND_DECODER_WAV "" ON)
option(SDLSOUND_DECODER_AIFF "" OFF)
option(SDLSOUND_DECODER_AU "" OFF)
option(SDLSOUND_DECODER_VOC "" OFF)
option(SDLSOUND_DECODER_FLAC "" OFF)
option(SDLSOUND_DECODER_VORBIS "" ON)
option(SDLSOUND_DECODER_RAW "" OFF)
option(SDLSOUND_DECODER_SHN "" OFF)
option(SDLSOUND_DECODER_MODPLUG "" OFF)
option(SDLSOUND_DECODER_MP3 "" OFF)
option(SDLSOUND_DECODER_MIDI "" OFF)
option(SDLSOUND_DECODER_COREAUDIO "" OFF)
option(SDLSOUND_BUILD_STATIC "" ON)
option(SDLSOUND_BUILD_SHARED "" OFF)
option(SDLSOUND_BUILD_TEST "" OFF)
add_subdirectory(deps/SDL_sound EXCLUDE_FROM_ALL)

# # SDL_nmix
add_library(SDL_nmix STATIC
  deps/SDL_nmix/SDL_nmix.c
  deps/SDL_nmix/SDL_nmix_file.c)
target_include_directories(SDL_nmix PRIVATE ${SDL2_INCLUDE_DIRS} deps/SDL_sound/src)
target_link_libraries(SDL_nmix PRIVATE ${SDL2_LIBRARIES} SDL2_sound-static)

# # DBGP
add_subdirectory(deps/DBGP EXCLUDE_FROM_ALL)

# SETUP SMGF
set(RESOURCE_FILES
  scripts/license.txt
)

if(NOT EMSCRIPTEN)
  # creating default "game.smgf" file (to be installed with app)
  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/game.smgf"
    COMMAND ${CMAKE_COMMAND}
    ARGS -E tar "cfv" "${CMAKE_CURRENT_BINARY_DIR}/game.smgf" --format=zip .
    WORKING_DIRECTORY "${GAME_PATH}"
  )

  set(RESOURCE_FILES "${RESOURCE_FILES}" "${CMAKE_CURRENT_BINARY_DIR}/game.smgf")
endif()

add_executable(SMGF
  deps/physfs/extras/physfsrwops.c # should not be here...
  src/smgf.c
  src/smgf_callbacks.c
  src/api/audio.c
  src/api/audio_lua.c
  src/api/graphics.c
  src/api/graphics_lua.c
  src/api/input.c
  src/api/input_lua.c
  src/api/io.c
  src/api/io_lua.c
  src/api/system.c
  src/api/system_lua.c
  src/api_lua.c
  src/main.c
  "${RESOURCE_FILES}"
)

set_target_properties(SMGF
  PROPERTIES C_STANDARD 99
  RESOURCE "${RESOURCE_FILES}"
)

if(MSVC)
  target_compile_options(SMGF PRIVATE /W4)
else()
  target_compile_options(SMGF PRIVATE -Wall -Wextra -pedantic)
endif()

target_compile_definitions(SMGF PRIVATE SMGF_VERSION="${SMGF_VERSION}")

add_dependencies(SMGF
  lua
  physfs-static
  SDL2_sound-static
  SDL_nmix
)

target_include_directories(SMGF PRIVATE
  ${SDL2_INCLUDE_DIRS}
  deps/lua-5.4.6/src
  deps/physfs/src
  deps/physfs/extras
  deps/SDL_sound/src
  deps/SDL_nmix
  deps/DBGP
  deps
  src
)

target_link_libraries(SMGF PRIVATE
  ${SDL2_LIBRARIES}
  lua
  physfs-static
  SDL2_sound-static
  SDL_nmix
  SDL_DBGP
)

# macOS options
set_target_properties(SMGF PROPERTIES
  MACOSX_BUNDLE TRUE

  MACOSX_BUNDLE_ICON_FILE "${PROJECT_NAME}.icns"
  MACOSX_BUNDLE_EXECUTABLE_NAME ${PROJECT_NAME}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
  MACOSX_BUNDLE_COPYRIGHT ""
  MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
  MACOSX_BUNDLE_GUI_IDENTIFIER "com.nicolasallemand.smgf"
  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"

  XCODE_EMBED_FRAMEWORKS "${SDL2_FRAMEWORK_PATH}"
  XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
  XCODE_EMBED_FRAMEWORKS_REMOVE_HEADERS_ON_COPY TRUE
  XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.nicolasallemand.smgf"

  # BUILD_WITH_INSTALL_RPATH ON
  # INSTALL_RPATH "@executable_path/../Frameworks"
  XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/../Frameworks"

  # hardened runtime requires proper code signing (does not allow adhoc signing)
  # XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME "YES"
)

# cmake writes binary to default location ("bin") before copying it to "."
# instead of modifying the installation path (bug?),
# so we override this value to fix this
set(CMAKE_INSTALL_BINDIR ".")

install(TARGETS SMGF
  BUNDLE DESTINATION .
  RUNTIME DESTINATION .
)

if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
  install(TARGETS SMGF
    RESOURCE DESTINATION .
  )
endif()

if(WIN32)
  install(IMPORTED_RUNTIME_ARTIFACTS SDL2::SDL2 RUNTIME DESTINATION .)
endif()

if(EMSCRIPTEN)
  # Emscripten does not copy js, wasm & data files (bug?):
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SMGF.js"
    "${CMAKE_CURRENT_BINARY_DIR}/SMGF.wasm"
    "${CMAKE_CURRENT_BINARY_DIR}/SMGF.data"
    DESTINATION .
  )
endif()

# clang-tidy (run cmake with -DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
# usage: `make tidy`
find_program(RUN_CLANG_TIDY_BIN run-clang-tidy)
list(APPEND RUN_CLANG_TIDY_BIN_ARGS
  -header-filter=".*"
)
add_custom_target(tidy
  COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS} src/
  COMMENT "Runs clang-tidy on project"
)
