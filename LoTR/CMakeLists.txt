cmake_minimum_required(VERSION 3.5)

project(LoTR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Sql REQUIRED)

add_executable(LoTR main.cpp
    hero.h hero.cpp
    enemy.h enemy.cpp
    interface.h interface.cpp
    character.h character.cpp
    cave.h cave.cpp)

include(GNUInstallDirs)
install(TARGETS LoTR
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(LoTR Qt5::Core Qt5::Sql)


