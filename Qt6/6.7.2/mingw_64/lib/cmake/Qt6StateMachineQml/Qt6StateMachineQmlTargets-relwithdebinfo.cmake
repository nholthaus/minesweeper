#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::StateMachineQml" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::StateMachineQml APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::StateMachineQml PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libQt6StateMachineQml.a"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "Qt6::Qml"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6StateMachineQml.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::StateMachineQml )
list(APPEND _cmake_import_check_files_for_Qt6::StateMachineQml "${_IMPORT_PREFIX}/lib/libQt6StateMachineQml.a" "${_IMPORT_PREFIX}/bin/Qt6StateMachineQml.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
