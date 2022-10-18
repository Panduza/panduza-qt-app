target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
    
    Tree.cpp
    BrokerDeclaration.cpp
    InterfaceDeclaration.cpp
)

