# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Find "ModuleTools" dependencies, which are other ModuleTools packages.
set(Qt6WidgetsTools_FOUND FALSE)
set(__qt_WidgetsTools_tool_deps "Qt6CoreTools\;6.7.2;Qt6GuiTools\;6.7.2")
foreach(__qt_WidgetsTools_target_dep ${__qt_WidgetsTools_tool_deps})
    list(GET __qt_WidgetsTools_target_dep 0 __qt_WidgetsTools_pkg)
    list(GET __qt_WidgetsTools_target_dep 1 __qt_WidgetsTools_version)

    if (NOT ${__qt_WidgetsTools_pkg}_FOUND)
        find_dependency(${__qt_WidgetsTools_pkg} ${__qt_WidgetsTools_version})
    endif()
endforeach()

set(Qt6WidgetsTools_FOUND TRUE)
