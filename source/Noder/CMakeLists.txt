target_include_directories(${PROG} PRIVATE "./")
target_include_directories(${PROG} PRIVATE "./SidePanel")
target_include_directories(${PROG} PRIVATE "./Node")
target_include_directories(${PROG} PRIVATE "./Node/Pin")
target_include_directories(${PROG} PRIVATE "./Node/Nodes")

target_sources(${PROG}
	PRIVATE
	Noder.cpp
	NoderFrame.cpp
	NoderGraph.cpp
	NoderFunction.cpp
	NoderScenario.cpp
	NoderMenuBar.cpp
	NoderVariable.cpp
	NoderView.cpp
	NoderScene.cpp
	NoderConsole.cpp
	NoderOutputArea.cpp

	SidePanel/NoderSidePanel.cpp
	SidePanel/NoderSPFunction.cpp
	SidePanel/NoderSPPin.cpp
	SidePanel/NoderSPVarPicker.cpp
	SidePanel/NoderSPVariable.cpp

	Node/Pin/Pin.cpp
	Node/Pin/PinExec.cpp
	Node/Pin/PinArray.cpp
	Node/Pin/PinRef.cpp

	Node/GNode.cpp
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
