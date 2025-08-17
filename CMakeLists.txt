cmake_minimum_required(VERSION 3.30)
project(ilc-delta CXX)
set(CMAKE_CXX_STANDARD 23)

find_package(immer REQUIRED)

# default library

add_library(ilc-delta
    src/ilc-delta.hpp
    src/replaceable.hpp
    src/tuple.hpp
    src/multiset.hpp
    )

target_include_directories(ilc-delta PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(ilc-delta PUBLIC cxx_std_23)
set_target_properties(ilc-delta PROPERTIES LINKER_LANGUAGE "CXX")
target_link_libraries(ilc-delta immer::immer)

# executable for experiments and testing

add_executable(ilc-main src/main.cpp)

target_link_libraries(ilc-main ilc-delta)

install(TARGETS ilc-main ilc-delta DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )


# tests

add_subdirectory(tests)
