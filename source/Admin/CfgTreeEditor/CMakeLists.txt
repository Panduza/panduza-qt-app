add_subdirectory("ItemEditor")
add_subdirectory("CfgTreeTree")
add_subdirectory("PlatformTree")

target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
	CfgTreeEditor.cpp
)


