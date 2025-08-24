find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/replaceable_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tuple_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/multiset_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/set_test.cpp
)

add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain ilc-delta)
catch_discover_tests(unit_tests)

enable_testing()
add_test(NAME CatchTests COMMAND unit_tests)
