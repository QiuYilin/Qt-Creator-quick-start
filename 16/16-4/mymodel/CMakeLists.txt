
add_executable(mymodel2
"main.cpp" "stringlistmodel.cpp" "stringlistmodel.h")

target_link_libraries(mymodel2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(mymodel2 PROPERTIES
          CXX_STANDARD 17
          #CXX_EXTENSIONS OFF
          )