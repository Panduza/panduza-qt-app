target_include_directories(${PROG} PRIVATE "./")

target_sources(${PROG}
	PRIVATE
    PlatformTreeItem.cpp
    PlatformTreeModel.cpp
    PlatformTreeMimeData.cpp
)
