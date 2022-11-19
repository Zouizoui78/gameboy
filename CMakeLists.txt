cmake_minimum_required(VERSION 3.0.0)
project(emu-gbc VERSION 0.1.0)

# C++ variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# OS defines used inside code
if (WIN32)
    add_compile_options(-DWINDOWS)
endif (WIN32)
if (UNIX)
    add_compile_options(-DLINUX)
endif (UNIX)

add_compile_options($<$<CONFIG:Debug>:-DDEBUG>$<$<CONFIG:Release>:-DRELEASE>)

set(
    SRC
    src/main.cpp
    src/CPU.cpp
    src/Instruction.cpp
)

add_executable(emu-gbc ${SRC})

target_include_directories(
    emu-gbc
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(tools)
target_link_libraries(
    emu-gbc
    tools
)

add_custom_target(
    run
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/emu-gbc
    DEPENDS emu-gbc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)