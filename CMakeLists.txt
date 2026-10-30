cmake_minimum_required(VERSION 3.20)
project(PS4PS5TransferTool VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(transfer_core
    src/backup_engine.cpp
    src/gui_model.cpp
    src/platform_stub.cpp
    src/sha256.cpp
)
target_include_directories(transfer_core PUBLIC include)
if(MSVC)
    target_compile_options(transfer_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(transfer_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(transfer_tool src/main.cpp)
target_link_libraries(transfer_tool PRIVATE transfer_core)

enable_testing()
add_test(NAME cli_help COMMAND transfer_tool --help)
