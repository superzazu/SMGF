cmake_minimum_required(VERSION 3.18)

set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "")

# Universal build for macOS
if(CMAKE_GENERATOR STREQUAL "Xcode")
  set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)" CACHE STRING "Target architectures for macOS")
else()
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Target architectures for macOS")
endif()

set(GAME_PATH "${CMAKE_CURRENT_SOURCE_DIR}/games/default-game" CACHE STRING "Path to the game to bundle")

project(SMGF
  VERSION 2.0.0
  LANGUAGES C
  DESCRIPTION "A lightweight, portable and open-source 2D game engine"
  HOMEPAGE_URL "https://github.com/superzazu/smgf")

message(STATUS
  "Configuring SMGF ${SMGF_VERSION} for ${CMAKE_SYSTEM_NAME} "
  "(generator: ${CMAKE_GENERATOR}, cross-compiling: ${CMAKE_CROSSCOMPILING}, CMake version: ${CMAKE_VERSION})")
message(STATUS "Bundling game ${GAME_PATH}")

# SETUP DEPENDENCIES

include(FetchContent)
# set(FETCHCONTENT_QUIET FALSE)

# if(APPLE)
#   # option(SDL_FRAMEWORK "Build SDL libraries as Apple Framework" ON)
#   # set(SDL_FRAMEWORK ON CACHE BOOL "Build SDL libraries as Apple Framework")
#   set(SDL_FRAMEWORK ON)
# endif()

# prevent building SDL_image & SDL_mixer as shared libraries
set(BUILD_SHARED_LIBS FALSE)

find_package(SDL3 QUIET)
if(SDL3_FOUND)
  message(STATUS "Using SDL3 from system")
else()
  message(STATUS "Using SDL3 via FetchContent")
endif()

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG origin/main
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
  EXCLUDE_FROM_ALL
  FIND_PACKAGE_ARGS
)

# option(SDLIMAGE_DEPS_SHARED "Load dependencies dynamically" OFF)
option(SDLIMAGE_VENDORED "Use vendored third-party libraries" ON)
option(SDLIMAGE_AVIF "Support loading AVIF images" OFF)
option(SDLIMAGE_BMP "Support loading BMP images" OFF)
option(SDLIMAGE_GIF "Support loading GIF images" OFF)
option(SDLIMAGE_JPG "Support loading JPEG images" OFF)
option(SDLIMAGE_JXL "Support loading JXL images" OFF)
option(SDLIMAGE_LBM "Support loading LBM images" OFF)
option(SDLIMAGE_PCX "Support loading PCX images" OFF)
option(SDLIMAGE_PNG "Support loading PNG images" ON)
option(SDLIMAGE_PNM "Support loading PNM images" OFF)
option(SDLIMAGE_QOI "Support loading QOI images" OFF)
option(SDLIMAGE_SVG "Support loading SVG images" OFF)
option(SDLIMAGE_TGA "Support loading TGA images" OFF)
option(SDLIMAGE_TIF "Support loading TIFF images" OFF)
option(SDLIMAGE_WEBP "Support loading WEBP images" OFF)
option(SDLIMAGE_XCF "Support loading XCF images" OFF)
option(SDLIMAGE_XPM "Support loading XPM images" OFF)
option(SDLIMAGE_XV "Support loading XV images" OFF)
option(SDLIMAGE_BACKEND_IMAGEIO "Use native Mac OS X frameworks for loading images?" OFF)

FetchContent_Declare(
  SDL3_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG origin/release-3.2.x
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
  GIT_SUBMODULES ""
  EXCLUDE_FROM_ALL
  # FIND_PACKAGE_ARGS
)
message(STATUS "Using SDL3_image via FetchContent")

option(SDLMIXER_VENDORED "Use vendored third-party libraries" ON)
option(SDLMIXER_FLAC_LIBFLAC "Enable FLAC audio using libFLAC" OFF)
option(SDLMIXER_FLAC_DRFLAC "Enable FLAC audio using drflac" OFF)
option(SDLMIXER_GME "Support loading GME music via game-music-emu" OFF)
option(SDLMIXER_MOD_XMP "Support loading MOD audio via libxmp" OFF)
option(SDLMIXER_MOD "Support loading MOD music" OFF)
option(SDLMIXER_MP3_DRMP3 "Support loading MP3 audio via dr_mp3" OFF)
option(SDLMIXER_MP3_MPG123 "Support loading MP3 audio via libmpg123" OFF)
option(SDLMIXER_MIDI_TIMIDITY "Support timidity MIDI output" OFF)
option(SDLMIXER_OPUS "Enable Opus music" OFF)
option(SDLMIXER_VORBIS_STB "Enable Ogg Vorbis audio via stb_vorbis" ON)
option(SDLMIXER_VORBIS_VORBISFILE "Enable Ogg Vorbis audio via libvorbisfile" OFF)
option(SDLMIXER_WAVPACK "Enable WavPack audio" OFF)
option(SDLMIXER_AIFF "Enable AIFF audio" ON)
option(SDLMIXER_WAVE "Enable WAVE audio" ON)
option(SDLMIXER_VOC "Enable VOC audio" OFF)
option(SDLMIXER_AU "Enable AU audio" OFF)

FetchContent_Declare(
  SDL3_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG origin/main
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
  GIT_SUBMODULES "external/ogg;external/vorbis"
  EXCLUDE_FROM_ALL
  # FIND_PACKAGE_ARGS
)
message(STATUS "Using SDL3_mixer via FetchContent")

FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_mixer)

if(EMSCRIPTEN)
  # we tell emscripten we want a html file alongside the javascript file:
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${GAME_PATH}@/game")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s MAXIMUM_MEMORY=1gb")
endif()

## Lua
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
add_library(lua STATIC EXCLUDE_FROM_ALL ${LUA_SRCS})

target_include_directories(lua PRIVATE deps/lua-5.4.6/src)

## physfs
option(PHYSFS_ARCHIVE_ZIP "" ON)
option(PHYSFS_ARCHIVE_7Z "" OFF)
option(PHYSFS_ARCHIVE_GRP "" OFF)
option(PHYSFS_ARCHIVE_WAD "" OFF)
option(PHYSFS_ARCHIVE_CSM "" OFF)
option(PHYSFS_ARCHIVE_HOG "" OFF)
option(PHYSFS_ARCHIVE_MVL "" OFF)
option(PHYSFS_ARCHIVE_QPAK "" OFF)
option(PHYSFS_ARCHIVE_SLB "" OFF)
option(PHYSFS_ARCHIVE_ISO9660 "" OFF)
option(PHYSFS_ARCHIVE_VDF "" OFF)
option(PHYSFS_ARCHIVE_LECARCHIVES "" OFF)
option(PHYSFS_BUILD_STATIC "" ON)
option(PHYSFS_BUILD_SHARED "" OFF)
option(PHYSFS_BUILD_TEST "" OFF)
option(PHYSFS_BUILD_DOCS "" OFF)
option(PHYSFS_DISABLE_INSTALL "Disable installing PhysFS" ON)
set(PHYSFS_TARGETNAME_DOCS "physfs-docs")
set(PHYSFS_TARGETNAME_DIST "physfs-dist")
set(PHYSFS_TARGETNAME_UNINSTALL "physfs-uninstall")
add_subdirectory(deps/physfs EXCLUDE_FROM_ALL)

## DBGP
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
  deps/physfs/extras/physfssdl3.c
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
)

target_include_directories(SMGF PRIVATE
  deps/lua-5.4.6/src
  deps/physfs/src
  deps/physfs/extras
  deps/DBGP
  deps
  src
)

target_link_libraries(SMGF PRIVATE
  SDL3::SDL3
  SDL3_mixer::SDL3_mixer
  SDL3_image::SDL3_image
  lua
  physfs-static
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

  # https://gitlab.kitware.com/cmake/cmake/-/issues/25487
  # XCODE_EMBED_FRAMEWORKS SDL3::SDL3
  XCODE_EMBED_FRAMEWORKS "~/Library/Frameworks/SDL3.xcframework/macos-arm64_x86_64/SDL3.framework"
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
# set(CMAKE_INSTALL_BINDIR ".")


if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
  install(TARGETS SMGF DESTINATION "")
  # install(TARGETS SMGF
  #   # RESOURCE DESTINATION .
  #   # BUNDLE DESTINATION .
  #   # RUNTIME DESTINATION .
  #   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  #   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  #   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  # )
else()
  install(TARGETS SMGF DESTINATION .)
  # install(TARGETS SMGF
  #   BUNDLE DESTINATION .
  #   RUNTIME DESTINATION .
  # )
endif()

# if(WIN32)
#   install(IMPORTED_RUNTIME_ARTIFACTS SDL2::SDL2 RUNTIME DESTINATION .)
# endif()

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
