cmake_minimum_required(VERSION 3.14)

project(${PROG} LANGUAGES CXX)

set(PROG panduza-qt-app)

# External dependencies
include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/../tools)
set(FETCHCONTENT_QUIET FALSE)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/dependencies.cmake)






FetchContent_MakeAvailable(yaml-cpp)



set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-function")

find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Mqtt REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Concurrent REQUIRED)

find_library(HAVE_SSH NAMES ssh)

FILE(GLOB_RECURSE INCLUDE *.hpp)

add_executable(${PROG})

add_subdirectory("${PROJECT_SOURCE_DIR}/source")


target_include_directories(${PROG} PRIVATE "${yaml-cpp_SOURCE_DIR}/include" )


target_sources(${PROG}
	PRIVATE
	${INCLUDE}
	resources.qrc
)

target_link_libraries(${PROG}
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Mqtt
	Qt${QT_VERSION_MAJOR}::Svg
	Qt${QT_VERSION_MAJOR}::Concurrent
	ssh
	yaml-cpp
)

install(TARGETS ${PROG})
