
add_subdirectory(etcpza)

target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
	Workspace.cpp
)
