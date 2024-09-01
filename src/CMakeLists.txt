set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(appinfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/appinfo.h @ONLY NEWLINE_STYLE UNIX)

qt_add_resources(RESOURCES ../resources/resources.qrc)

qt6_add_executable(${PROJECT_NAME}
               gameboard.h
               gameboard.cpp
               highScore.cpp
               highScore.h
               highScoreDialog.cpp
               highScoreDialog.h
               highScoreModel.cpp
               highScoreModel.h
               main.cpp
               mainwindow.cpp
               mainwindow.h
               mineCounter.h
               mineCounter.cpp
               minetimer.cpp
               minetimer.h
               tile.cpp
               tile.h
               ../resources/resources.rc
               ${RESOURCES}
               ${CMAKE_CURRENT_BINARY_DIR}/appinfo.h
               )

target_link_libraries(${PROJECT_NAME} PRIVATE
                      Qt::Core
                      Qt::Gui
                      Qt::Widgets
                      Qt::StateMachine
                      Qt::Concurrent
                      Threads::Threads
                      )

set_target_properties(${PROJECT_NAME} PROPERTIES
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      WIN32_EXECUTABLE ON
                      )

install(TARGETS ${PROJECT_NAME}
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

#-------------------------------------------------------------------------------
#	DEPLOY DLLS
#-------------------------------------------------------------------------------

include(windeployqt)
include(InstallRequiredSystemLibraries)
windeployqt(${PROJECT_NAME})
qt_generate_deploy_app_script(
		TARGET ${PROJECT_NAME}
		OUTPUT_SCRIPT deploy_script
		NO_UNSUPPORTED_PLATFORM_ERROR
		)
install(SCRIPT ${deploy_script})

#-------------------------------------------------------------------------------
#	INSTALLER
#-------------------------------------------------------------------------------

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "Minesweeper")
set(CPACK_PACKAGE_VENDOR "Menari Softworks")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Free Qt/C++17 Minesweeper")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/nholthaus/minesweeper")
set(CPACK_PACKAGE_VERSION ${GIT_TAG})

INCLUDE(CPack)