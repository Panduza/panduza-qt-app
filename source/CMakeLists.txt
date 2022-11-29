add_subdirectory("Noder")
add_subdirectory("Store")
add_subdirectory("StoreBar")
add_subdirectory("Admin")

target_include_directories(${PROG} PRIVATE ".")
target_include_directories(${PROG} PRIVATE "Utils")
target_include_directories(${PROG} PRIVATE "Widgets")
target_include_directories(${PROG} PRIVATE "StoreBar")

target_sources(${PROG}
	PRIVATE
	main.cpp

	MainWindow.cpp
	MainStatusBar.cpp
	CentralWidget.cpp

	Widgets/PzaWidget.cpp
	Widgets/PzaSpoiler.cpp
	Widgets/PzaMoreLess.cpp
	Widgets/PzaLineEdit.cpp
	Widgets/PzaLabel.cpp
	Widgets/PzaForm.cpp
	Widgets/PzaCheckBox.cpp
	Widgets/PzaSpinBox.cpp
	Widgets/PzaDoubleSpinBox.cpp
	Widgets/PzaMenu.cpp
	Widgets/PzaMenuBar.cpp
	Widgets/PzaComboBox.cpp
	Widgets/PzaTable.cpp
	Widgets/PzaSplitter.cpp
	Widgets/PzaScrollArea.cpp
	Widgets/PzaColorBox.cpp
	Widgets/PzaStackedWidget.cpp
	Widgets/PzaTabWidget.cpp
	Widgets/PzaTextEdit.cpp
	Widgets/PzaPushButton.cpp
	Widgets/PzaToolButton.cpp
	Widgets/PzaPopup.cpp
	Widgets/PzaTreeWidget.cpp
	Widgets/PzaTreeWidgetItem.cpp
	Widgets/PzaNameEntry.cpp
	Widgets/PzaAdminButton.cpp
)
