cmake_minimum_required(VERSION 3.28)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME SpectralToolbox)
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(spdlog REQUIRED)

add_subdirectory(lib/common)
add_subdirectory(lib/io)
add_subdirectory(lib/model)

project(${PROJECT_NAME} LANGUAGES CXX)
add_subdirectory(lib/emd)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/widgets/mainwindow.cpp
    src/widgets/procedure_config_widget.cpp
    include/widgets/mainwindow.hpp
    include/widgets/procedure_config_widget.hpp
    src/widgets/mainwindow.ui
    include/widgets/file_widget.hpp
    src/widgets/file_widget.ui
    src/widgets/file_widget.cpp
    src/widgets/tracedata.cpp
    include/widgets/tracedata.hpp
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    spdlog

  common_lib
  io_lib
  model_lib
PRIVATE
  Qt::Core
  Qt::Widgets
)

# Линковка со статической библиотекой
target_link_libraries(${PROJECT_NAME} PRIVATE common_lib io_lib emd_lib)
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# подключение тестов
enable_testing()
include(CTest)
if (BUILD_TESTING)
  add_subdirectory(test)
endif(BUILD_TESTING)

