add_subdirectory("CfgTreeTree")
add_subdirectory("PlatformTree")
add_subdirectory("ItemEditor")

target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
	CfgTreeEditor.cpp
)


