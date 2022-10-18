add_subdirectory("Platform")

target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE

	AdminFrame.cpp
)


