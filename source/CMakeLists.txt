add_subdirectory("Panduza")
add_subdirectory("Noder")

set(LOCAL_SOURCE
	main.cpp
	MainWindow.cpp
)

set(LOCAL_SOURCE_APPEND "")

foreach(SRC ${LOCAL_SOURCE})
	set(LOCAL_SOURCE_APPEND ${LOCAL_SOURCE_APPEND} "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
endforeach()

set(SOURCE "${SOURCE}" "${LOCAL_SOURCE_APPEND}" PARENT_SCOPE)