target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
    
    CfgTree.cpp
    CfgBroker.cpp
    # CfgInterface.cpp
)

