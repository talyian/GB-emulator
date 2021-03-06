project(gb_emulator)
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_COMPILER clang++-8)
set(CMAKE_C_COMPILER   clang++-8)

if (WASM)
  set(CMAKE_LINKER       wasm-ld-8)
  set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

  add_compile_options(
    -DWASM=1
    --target=wasm32
    -fvisibility=hidden
    -std=c++14
    -Wall
    -O2
    -Wno-unused-variable
    -nostdlib
    -ffunction-sections
    -fdata-sections
  )

  add_link_options(
    --export-dynamic
    --no-entry
    --strip-all
    --gc-sections
    --allow-undefined
    --import-memory
)

add_executable(gb_emulator.wasm gb/lib_gb.cpp host.cpp)
add_custom_target(copy_wasm
  COMMAND cp gb_emulator.wasm ../../wasmgui/build/
  DEPENDS gb_emulator.wasm)

elseif (WINDOWS)
  add_executable(gb_emulator gb/lib_gb.cpp host.cpp)
  add_executable(gb_test gb/lib_gb.cpp test/test.cpp)
  target_link_libraries(gb_emulator OpenGL32)
else()
  add_compile_options(
    -fvisibility=hidden
    -std=c++14
    -Wall
    -Wno-unused-variable
    -g
    -O1
    -fsanitize=undefined
    -I ${CMAKE_SOURCE_DIR}
    -I ${CMAKE_SOURCE_DIR}/utils
  )

  add_link_options(
    -fsanitize=undefined
    -flto
    -O1
  )

  add_executable(gb_emulator_console gb/lib_gb.cpp host.cpp)
  add_custom_target(run
    COMMAND ./gb_emulator_console
    DEPENDS gb_emulator_console)
endif()
