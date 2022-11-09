target_include_directories(${PROG} PRIVATE "./")
target_include_directories(${PROG} PRIVATE "./Node")
target_include_directories(${PROG} PRIVATE "./Node/Nodes")

target_sources(${PROG}
	PRIVATE
	Noder.cpp
	NoderFrame.cpp
	NoderGraph.cpp
	NoderFunction.cpp
	NoderScenario.cpp
	NoderMenuBar.cpp
	NoderSP.cpp
	NoderSPVariable.cpp
	NoderView.cpp
	NoderScene.cpp
	NoderConsole.cpp
	NoderOutputArea.cpp
	NoderStyle.cpp

	Node/GNode.cpp
	Node/Pin.cpp
	Node/Link.cpp
	Node/GNodeInstance.cpp
	Node/GNodeBranch.cpp
	Node/GNodeBasic.cpp
	Node/GNodeOp.cpp
	Node/GNodeEvent.cpp
	Node/GNodeExec.cpp

	Node/Nodes/NFDelay.cpp
	Node/Nodes/NFString.cpp
	Node/Nodes/NFArray.cpp
	Node/Nodes/NFIf.cpp
	Node/Nodes/NFFor.cpp
	Node/Nodes/NFEvent.cpp
	Node/Nodes/NFMath.cpp
	Node/Nodes/NFIO.cpp
	Node/Nodes/NFInstance.cpp
)
