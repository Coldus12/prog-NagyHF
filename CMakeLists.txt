cmake_minimum_required(VERSION 3.6)
project(nagyHF C)

set(CMAKE_C_STANDARD 99)

add_executable(nagyHF debugmalloc.h debugmalloc-impl.h main.c object.c object.h renderer.c renderer.h map.c map.h)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Find and link SDL2
find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Find and link SDL2_image
find_package(SDL2_image REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})

# Find and link SDL2_gfx
find_package(SDL2_gfx REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_GFX_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_GFX_LIBRARIES})

target_link_libraries(${PROJECT_NAME} m)