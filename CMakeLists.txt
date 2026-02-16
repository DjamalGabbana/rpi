cmake_minimum_required(VERSION 3.10)

project(temp-rpi3 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(temp-rpi3 ${SOURCES})

target_link_libraries(temp-rpi3
    pigpio
    pthread
    paho-mqttpp3
    paho-mqtt3as
)
