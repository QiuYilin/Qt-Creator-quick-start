file(GLOB_RECURSE CPPS  ./*.cpp )
file(GLOB_RECURSE HS  ./*.h )
#file(GLOB_RECURSE UIS  ./*.ui )

add_executable(nested_mvd ${CPPS} ${HS} "mainwindow.ui")

target_link_libraries(nested_mvd PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(nested_mvd PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

set_target_properties(nested_mvd PROPERTIES
          CXX_STANDARD 17
          #CXX_EXTENSIONS OFF
          )