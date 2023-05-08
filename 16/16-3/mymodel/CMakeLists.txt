
add_executable(mymodel
"main.cpp" "stringlistmodel.cpp" "stringlistmodel.h")

target_link_libraries(mymodel PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(mymodel PROPERTIES
          CXX_STANDARD 17
          #CXX_EXTENSIONS OFF
          )