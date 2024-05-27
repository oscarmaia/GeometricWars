cmake_minimum_required(VERSION 3.25)

set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(GeometricWars)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

# Create link for the compilation database `compile_commands.json`.
set(COMPILE_COMMANDS_JSON "${CMAKE_CURRENT_BINARY_DIR}/build/compile_commands.json")
set(COMPILE_COMMANDS_JSON_LINK "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json")
file(CREATE_LINK "${COMPILE_COMMANDS_JSON}" "${COMPILE_COMMANDS_JSON_LINK}" SYMBOLIC)

add_executable(${PROJECT_NAME} src/main.cpp src/Vec2.cpp src/Entity.cpp src/EntityManager.cpp src/Game.cpp )

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-network sfml-graphics sfml-window)