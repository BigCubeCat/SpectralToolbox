cmake_minimum_required(VERSION 3.28)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME SpectralToolbox)
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(lib/common)

# Создание исполняемого файла
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Линковка со статической библиотекой
target_link_libraries(${PROJECT_NAME} PRIVATE common_lib)


# подключение тестов
enable_testing()
include(CTest)
if (BUILD_TESTING)
  add_subdirectory(test)
endif(BUILD_TESTING)

