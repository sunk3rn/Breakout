cmake_minimum_required(VERSION 3.5)

# Název projektu
project(hra)

# Přidání přepínačů překladače
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

# Vyhledání knihovny SDL
find_package(SDL2 REQUIRED)
message(STATUS "SDL2 found: ${SDL2_FOUND}")

find_package(SDL2_ttf REQUIRED)
if(NOT SDL2_ttf_FOUND)
  message(FATAL_ERROR "SDL2_ttf not found")
endif()
link_directories(${SDL2_TTF_LIBRARY_DIRS})

# Vytvoření programu s názvem `hra`
# Program se bude skládat ze dvou zadaných zdrojových souborů (jednotek překladu)
add_executable(hra hra.c)
#add_executable(hra hra.c grafika.c)

# Nastavte linker tak, aby vyhledával knihovnu SDL_ttf
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -L${SDL2_ttf_LIBRARY_DIRS}")

target_include_directories(hra PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(hra PRIVATE ${SDL2_LIBRARIES} SDL2_ttf)
