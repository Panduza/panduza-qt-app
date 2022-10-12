include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(LOCAL_SOURCE
	PanduzaEngine.cpp
	Group.cpp
	Machine.cpp
	Broker.cpp
	Interface/Io.cpp
	Interface/Platform.cpp
	Interface/Interface.cpp
)

set(LOCAL_SOURCE_APPEND "")

foreach(SRC ${LOCAL_SOURCE})
	set(LOCAL_SOURCE_APPEND ${LOCAL_SOURCE_APPEND} "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
endforeach()

set(SOURCE "${SOURCE}" "${LOCAL_SOURCE_APPEND}" PARENT_SCOPE)