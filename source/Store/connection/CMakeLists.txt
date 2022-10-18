
target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE

	AdminConnection.cpp
	SshConnection.cpp
	LocalWorker.cpp

    ConnectionInfo.cpp
    ConnectionLibrary.cpp
)
