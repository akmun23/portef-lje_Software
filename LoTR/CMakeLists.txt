cmake_minimum_required(VERSION 3.5)

project(LoTR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LoTR main.cpp
    hero.h hero.cpp
    enemy.h enemy.cpp)

include(GNUInstallDirs)
install(TARGETS LoTR
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
