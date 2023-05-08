add_executable(myselection
"main.cpp" "mainwindow.cpp""mainwindow.h" "mainwindow.ui")

target_link_libraries(myselection PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(myselection PROPERTIES
          CXX_STANDARD 17
          #CXX_EXTENSIONS OFF
          )