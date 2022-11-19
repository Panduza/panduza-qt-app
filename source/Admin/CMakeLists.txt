add_subdirectory("CfgTreeEditor")
add_subdirectory("System")

target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE

	AdminFrame.cpp
)


