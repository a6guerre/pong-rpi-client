cmake_minimum_required(VERSION 3.10)

project(pong-rpi-client)

add_subdirectory(sigfn)

if(WIN32)
    set(LIBENJOY_OS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/libenjoy/src/libenjoy_win32.c)
else()
    set(LIBENJOY_OS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/libenjoy/src/libenjoy_linux.c)
endif()

add_library(enjoy STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/libenjoy/src/libenjoy.c
    ${LIBENJOY_OS_SRC})

target_include_directories(enjoy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libenjoy/src)

add_executable(pong-rpi-client ${CMAKE_CURRENT_SOURCE_DIR}/src/pong-rpi-client.cpp)

target_link_libraries(pong-rpi-client PRIVATE sigfn++_a enjoy)