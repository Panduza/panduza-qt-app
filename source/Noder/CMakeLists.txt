include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS "PROUT" "${CMAKE_CURRENT_SOURCE_DIR}")

set(LOCAL_SOURCE
	NoderFrame.cpp
	NoderView.cpp
	NoderStyle.cpp
	Node/Nodes/NFDelay.cpp
	Node/Nodes/NFString.cpp
	Node/Nodes/NFArray.cpp
	Node/Nodes/NFIf.cpp
	Node/Nodes/NFFor.cpp
	Node/Nodes/NFEvent.cpp
	Node/Nodes/NFMath.cpp
	Node/Nodes/NFIO.cpp
	Node/GNodeInstance.cpp
	Node/Pin.cpp
	Node/GNodeBranch.cpp
	Node/GNode.cpp
	Node/GNodeOp.cpp
	Node/GNodeEvent.cpp
	Node/GNodeExec.cpp
	Node/Link.cpp
	NoderScene.cpp
	NoderDataBase.cpp
)

set(LOCAL_SOURCE_APPEND "")

foreach(SRC ${LOCAL_SOURCE})
	set(LOCAL_SOURCE_APPEND ${LOCAL_SOURCE_APPEND} "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
endforeach()

set(SOURCE "${SOURCE}" "${LOCAL_SOURCE_APPEND}" PARENT_SCOPE)