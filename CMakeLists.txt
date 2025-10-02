cmake_minimum_required(VERSION 3.19)
project(Bouncing_Balls)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    main.cpp
    ball.cpp
    game.cpp
)

# Find system-installed SFML
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
)