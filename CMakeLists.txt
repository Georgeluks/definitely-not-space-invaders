cmake_minimum_required(VERSION 3.16)
project(SpaceInvaders CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

add_executable(space_invaders 
  src/main.cpp 
  src/Game.cpp 
  src/Entity.cpp 
  src/Player.cpp 
  src/Bullet.cpp 
  src/Enemy.cpp 
  src/EnemyGrid.cpp
  src/TextRenderer.cpp
  src/TextureManager.cpp
  src/AudioManager.cpp
)

target_include_directories(space_invaders PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(space_invaders PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
