cmake_minimum_required(VERSION 3.30)
project(ilc-delta CXX)


find_package(immer REQUIRED)

add_executable(ilc-delta src/ilc-delta.cpp src/main.cpp)

target_link_libraries(ilc-delta immer::immer)

install(TARGETS ilc-delta DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
