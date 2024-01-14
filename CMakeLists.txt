cmake_minimum_required(VERSION 3.26.4)
project(BrickBreaker C)

set(CMAKE_C_STANDARD 11)

set(SDL2_PATH "C:\\Users\\schmi\\Documents\\SDL\\SDL2-2.28.5\\x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "C:\\Users\\schmi\\Documents\\SDL\\SDL2_image-2.6.3\\x86_64-w64-mingw32")
set(SDL2_TTF_PATH "C:\\Users\\schmi\\Documents\\SDL\\SDL2_ttf-2.20.2\\x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})


add_executable(BrickBreaker main.c
        MainMenu.c
        MainMenu.h
        EasterEgg.c
        EasterEgg.h
        game.c
        game.h
        SettingsMenu.c
        SettingsMenu.h
        InformationMenu.c
        InformationMenu.h
        MenuWin.c
        MenuWin.h
        MenuLoose.c
        MenuLoose.h
        MenuEasterEgg.c
        MenuEasterEgg.h)

target_link_libraries(BrickBreaker ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})