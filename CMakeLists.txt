cmake_minimum_required(VERSION 3.14)

project(${PROG} LANGUAGES CXX)

set(PROG gui2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Mqtt REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Svg REQUIRED)

set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
	${SOURCE_FOLDER}/main.cpp

	${SOURCE_FOLDER}/MainWindow.cpp

	${SOURCE_FOLDER}/PanduzaEngine.cpp
	${SOURCE_FOLDER}/Broker.cpp
	${SOURCE_FOLDER}/Machine.cpp
	${SOURCE_FOLDER}/Group.cpp

	${SOURCE_FOLDER}/Noder/Pin.cpp
	${SOURCE_FOLDER}/Noder/Node/Link.cpp

	${SOURCE_FOLDER}/Noder/Node/GNode.cpp
	${SOURCE_FOLDER}/Noder/Node/GNodeExec.cpp
	${SOURCE_FOLDER}/Noder/Node/GNodeOp.cpp
	${SOURCE_FOLDER}/Noder/Node/GNodeEvent.cpp
	${SOURCE_FOLDER}/Noder/Node/GNodeBranch.cpp
	${SOURCE_FOLDER}/Noder/Node/GNodeInstance.cpp

	${SOURCE_FOLDER}/Noder/NoderFrame.cpp
	${SOURCE_FOLDER}/Noder/NoderScene.cpp
	${SOURCE_FOLDER}/Noder/NoderDataBase.cpp
	${SOURCE_FOLDER}/Noder/NoderView.cpp
	${SOURCE_FOLDER}/Noder/NoderStyle.cpp

	${SOURCE_FOLDER}/Interface/Interface.cpp

	${SOURCE_FOLDER}/Noder/Node/Nodes/NFMath.cpp
	${SOURCE_FOLDER}/Noder/Node/Nodes/NFArray.cpp
	${SOURCE_FOLDER}/Noder/Node/Nodes/NFString.cpp
	${SOURCE_FOLDER}/Noder/Node/Nodes/NFIf.cpp
	${SOURCE_FOLDER}/Noder/Node/Nodes/NFFor.cpp
	${SOURCE_FOLDER}/Noder/Node/Nodes/NFEvent.cpp
	${SOURCE_FOLDER}/Noder/Node/Nodes/NFDelay.cpp
	${SOURCE_FOLDER}/Noder/Node/Nodes/NFIO.cpp
)

FILE(GLOB_RECURSE INCLUDE_FILES *.hpp)

include_directories(
	${INCLUDE_FOLDER}
	${INCLUDE_FOLDER}/Noder
	${INCLUDE_FOLDER}/Noder/Node
	${INCLUDE_FOLDER}/Noder/NodeComponent
	${INCLUDE_FOLDER}/Interface
	${SOURCE_FOLDER}/Noder/Node/Nodes
)

add_executable(${PROG}
	${SOURCE_FILES}
	${INCLUDE_FILES}
	resources.qrc
)

target_link_libraries(${PROG}
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Mqtt
	Qt${QT_VERSION_MAJOR}::Svg
)

install(TARGETS ${PROG})
