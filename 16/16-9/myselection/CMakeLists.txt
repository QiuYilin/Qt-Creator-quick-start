add_executable(myselection2
"main.cpp" "mainwindow.cpp""mainwindow.h" "mainwindow.ui" "spinboxdelegate.cpp" "spinboxdelegate.h")

target_link_libraries(myselection2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(myselection2 PROPERTIES
          CXX_STANDARD 17
          #CXX_EXTENSIONS OFF
          )