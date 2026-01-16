# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Find "ModuleTools" dependencies, which are other ModuleTools packages.
set(Qt6GuiTools_FOUND FALSE)
set(__qt_GuiTools_tool_deps "Qt6CoreTools\;6.7.2")
foreach(__qt_GuiTools_target_dep ${__qt_GuiTools_tool_deps})
    list(GET __qt_GuiTools_target_dep 0 __qt_GuiTools_pkg)
    list(GET __qt_GuiTools_target_dep 1 __qt_GuiTools_version)

    if (NOT ${__qt_GuiTools_pkg}_FOUND)
        find_dependency(${__qt_GuiTools_pkg} ${__qt_GuiTools_version})
    endif()
endforeach()

set(Qt6GuiTools_FOUND TRUE)
