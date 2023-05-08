#add_executable(nontype_param2
  #nontype_param2.cpp)

  add_executable(modelview2
  "main.cpp" )

target_link_libraries(modelview2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(modelview2 PROPERTIES
            CXX_STANDARD 17
            #CXX_EXTENSIONS OFF
            )