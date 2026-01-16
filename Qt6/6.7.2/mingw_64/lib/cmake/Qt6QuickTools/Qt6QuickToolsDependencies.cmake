# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Find "ModuleTools" dependencies, which are other ModuleTools packages.
set(Qt6QuickTools_FOUND FALSE)
set(__qt_QuickTools_tool_deps "Qt6QmlTools\;6.7.2")
foreach(__qt_QuickTools_target_dep ${__qt_QuickTools_tool_deps})
    list(GET __qt_QuickTools_target_dep 0 __qt_QuickTools_pkg)
    list(GET __qt_QuickTools_target_dep 1 __qt_QuickTools_version)

    if (NOT ${__qt_QuickTools_pkg}_FOUND)
        find_dependency(${__qt_QuickTools_pkg} ${__qt_QuickTools_version})
    endif()
endforeach()

set(Qt6QuickTools_FOUND TRUE)
