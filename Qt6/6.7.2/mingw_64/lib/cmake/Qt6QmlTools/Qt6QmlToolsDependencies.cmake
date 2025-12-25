# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Find "ModuleTools" dependencies, which are other ModuleTools packages.
set(Qt6QmlTools_FOUND FALSE)
set(__qt_QmlTools_tool_deps "")
foreach(__qt_QmlTools_target_dep ${__qt_QmlTools_tool_deps})
    list(GET __qt_QmlTools_target_dep 0 __qt_QmlTools_pkg)
    list(GET __qt_QmlTools_target_dep 1 __qt_QmlTools_version)

    if (NOT ${__qt_QmlTools_pkg}_FOUND)
        find_dependency(${__qt_QmlTools_pkg} ${__qt_QmlTools_version})
    endif()
endforeach()

set(Qt6QmlTools_FOUND TRUE)
