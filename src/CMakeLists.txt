cmake_minimum_required(VERSION 3.30)
project(Asciify C)

set(CMAKE_BUILD_TYPE Debug)
add_executable(asciify main.c helpers.c filter.c string.c image.c)

target_link_libraries(asciify -lm)
