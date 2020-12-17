cmake_minimum_required(VERSION 3.9)

project(mangadex_fetcher)

# Find required libraries.
find_package(LibArchive REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Find all project files.
file(GLOB_RECURSE SOURCES "src/**.cpp")
file(GLOB_RECURSE HEADERS "src/**.h")

# Setup executable.
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Tell linker which libraries to pull in.
target_link_libraries( ${PROJECT_NAME}
    curl
    archive
)

# Tell CMake how to install.
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
