target_include_directories(${PROG} PRIVATE "./")
target_include_directories(${PROG} PRIVATE "./Node")
target_include_directories(${PROG} PRIVATE "./Node/Nodes")

target_sources(${PROG}
	PRIVATE
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