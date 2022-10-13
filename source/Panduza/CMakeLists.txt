target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
	PanduzaEngine.cpp
	Group.cpp
	Machine.cpp
	Broker.cpp
	Interface/Interface.cpp
)