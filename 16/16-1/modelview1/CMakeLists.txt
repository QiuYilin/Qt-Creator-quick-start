#add_executable(nontype_param2
  #nontype_param2.cpp)

add_executable(modelview1
  "main.cpp" )

target_link_libraries(modelview1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(modelview1 PROPERTIES
            CXX_STANDARD 17
            #CXX_EXTENSIONS OFF
            )