# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Find "ModuleTools" dependencies, which are other ModuleTools packages.
set(Qt6ToolsTools_FOUND FALSE)
set(__qt_ToolsTools_tool_deps "")
foreach(__qt_ToolsTools_target_dep ${__qt_ToolsTools_tool_deps})
    list(GET __qt_ToolsTools_target_dep 0 __qt_ToolsTools_pkg)
    list(GET __qt_ToolsTools_target_dep 1 __qt_ToolsTools_version)

    if (NOT ${__qt_ToolsTools_pkg}_FOUND)
        find_dependency(${__qt_ToolsTools_pkg} ${__qt_ToolsTools_version})
    endif()
endforeach()

set(Qt6ToolsTools_FOUND TRUE)
