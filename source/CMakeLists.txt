add_subdirectory("Panduza")
add_subdirectory("Noder")

target_include_directories(${PROG} PRIVATE "Utils")
target_include_directories(${PROG} PRIVATE "Widgets")

target_sources(${PROG}
	PRIVATE
	main.cpp
	MainWindow.cpp

	Widgets/PzaWidget.cpp
	Widgets/PzaSpoiler.cpp
	Widgets/PzaMoreLess.cpp
	Widgets/PzaLineEdit.cpp
	Widgets/PzaProperty.cpp
	Widgets/PzaLabel.cpp
	Widgets/PzaPropertyTable.cpp
)
