
add_subdirectory(workspace)
add_subdirectory(connection)

target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
	Group.cpp
	Machine.cpp
	Broker.cpp
	Interface/Interface.cpp
	Style.cpp

	Store.cpp
)
