# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include_guard(DIRECTORY)

# Qml plugin targets might have dependencies on other qml plugin targets, but the Targets.cmake
# files are included in the order that file(GLOB) returns, which means certain targets that are
# referenced might not have been created yet, and ${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
# might be set to a message saying those targets don't exist.
#
# Postpone checking of which targets don't exist until all Qml PluginConfig.cmake files have been
# included, by including all the files one more time and checking for errors at each step.
#
# TODO: Find a better way to deal with this, perhaps by using find_package() instead of include
# for the Qml PluginConfig.cmake files.

# Distributions should probably change this default.
if(NOT DEFINED QT_SKIP_AUTO_QML_PLUGIN_INCLUSION)
    set(QT_SKIP_AUTO_QML_PLUGIN_INCLUSION OFF)
endif()

set(__qt_qml_plugins_config_file_list "")
set(__qt_qml_plugins_glob_prefixes "${CMAKE_CURRENT_LIST_DIR}")

# Allow passing additional prefixes where we will glob for PluginConfig.cmake files.
if(QT_ADDITIONAL_QML_PLUGIN_GLOB_PREFIXES)
    foreach(__qt_qml_plugin_glob_prefix IN LISTS QT_ADDITIONAL_QML_PLUGIN_GLOB_PREFIXES)
        if(__qt_qml_plugin_glob_prefix)
            list(APPEND __qt_qml_plugins_glob_prefixes "${__qt_qml_plugin_glob_prefix}")
        endif()
    endforeach()
endif()

list(REMOVE_DUPLICATES __qt_qml_plugins_glob_prefixes)

foreach(__qt_qml_plugin_glob_prefix IN LISTS __qt_qml_plugins_glob_prefixes)
    file(GLOB __qt_qml_plugins_glob_config_file_list
        "${__qt_qml_plugin_glob_prefix}/QmlPlugins/Qt6*Config.cmake")
    if(__qt_qml_plugins_glob_config_file_list)
        list(APPEND __qt_qml_plugins_config_file_list ${__qt_qml_plugins_glob_config_file_list})
    endif()
endforeach()

if (__qt_qml_plugins_config_file_list AND NOT QT_SKIP_AUTO_QML_PLUGIN_INCLUSION)
    # First round of inclusions ensure all qml plugin targets are brought into scope.
    foreach(__qt_qml_plugin_config_file ${__qt_qml_plugins_config_file_list})
        include(${__qt_qml_plugin_config_file})

        # Temporarily unset any failure markers and mark the Qml package as found.
        unset(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE)
        set(${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE)
    endforeach()

    # For the second round of inclusions, check and bail out early if there are errors.
    foreach(__qt_qml_plugin_config_file ${__qt_qml_plugins_config_file_list})
        include(${__qt_qml_plugin_config_file})

        if(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE)
            string(APPEND ${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
                "
The message was set in ${__qt_qml_plugin_config_file} ")
            set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
            return()
        endif()
    endforeach()

endif()

# Distributions should probably change this default.
if(NOT DEFINED QT_SKIP_AUTO_PLUGIN_INCLUSION)
    set(QT_SKIP_AUTO_PLUGIN_INCLUSION OFF)
endif()

if(NOT QT_NO_CREATE_TARGETS AND NOT QT_SKIP_AUTO_PLUGIN_INCLUSION)
    __qt_internal_include_plugin_packages(Qml)
endif()
