set(PROJECT_NAME test_spectral_toolbox)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${TEST_SOURCES})

SET(CMAKE_CXX_FLAGS "-g -Ofast -Wall -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -Ofast -Wall -W -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

add_executable(${PROJECT_NAME} ${TEST_SOURCES}
        test_idle.cpp)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC io_lib common_lib model_lib gtest)

