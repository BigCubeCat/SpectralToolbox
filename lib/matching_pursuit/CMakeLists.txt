set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LIB_NAME mp_lib)

project(${LIB_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Eigen3 REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(include)
file(GLOB_RECURSE SOURCES impl/*.hpp include/*.hpp)
file(GLOB_RECURSE HEADERS include/*.h)

add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        common_lib
        spdlog::spdlog
        Eigen3::Eigen
        OpenMP::OpenMP_CXX
)