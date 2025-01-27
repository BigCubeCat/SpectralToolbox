set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LIB_NAME common_lib)


project(${LIB_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


include_directories(include)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h include/*.hpp)

add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")
