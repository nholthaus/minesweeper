set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0087 NEW)
configure_file(appinfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/appinfo.h @ONLY NEWLINE_STYLE UNIX)

qt_add_resources(RESOURCES ../resources/resources.qrc)

qt6_add_executable(${PROJECT_NAME}
                   gameboard.h
                   gameboard.cpp
                   gameStats.cpp
                   gameStats.h
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
                   versionChecker.cpp
                   versionChecker.h
                   ../resources/resources.rc
                   ${RESOURCES}
                   ${CMAKE_CURRENT_BINARY_DIR}/appinfo.h
                   gameStatsDialog.cpp
                   gameStatsDialog.h
                   )

target_link_libraries(${PROJECT_NAME} PRIVATE
                      Qt::Core
                      Qt::Gui
                      Qt::Widgets
                      Qt::StateMachine
                      Qt::Concurrent
                      Qt::Network
                      Threads::Threads
                      )

if (WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES
	                      INSTALL_RPATH_USE_LINK_PATH TRUE
	                      WIN32_EXECUTABLE ON
	                      )
	install(TARGETS ${PROJECT_NAME}
	        RUNTIME DESTINATION bin
	        )
elseif (APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
	set_target_properties(${PROJECT_NAME} PROPERTIES
	                      MACOSX_BUNDLE_BUNDLE_NAME "Minesweeper"
	                      MACOSX_BUNDLE_BUNDLE_VERSION "${GIT_TAG}"
	                      MACOSX_BUNDLE_LONG_VERSION_STRING "${GIT_TAG}"
	                      MACOSX_BUNDLE_GUI_IDENTIFIER "github.com/nholthaus/minesweeper"
	                      )
	# Install the .app bundle
	install(TARGETS ${PROJECT_NAME}
	        BUNDLE DESTINATION ${CMAKE_BINARY_DIR}
	        COMPONENT Runtime)
endif (WIN32)

#-------------------------------------------------------------------------------
#	DEPLOY DLLS
#-------------------------------------------------------------------------------

include(InstallRequiredSystemLibraries)
if (WIN32)
	include(windeployqt)
	windeployqt(${PROJECT_NAME})
	qt_generate_deploy_app_script(
			TARGET ${PROJECT_NAME}
			OUTPUT_SCRIPT deploy_script
			NO_UNSUPPORTED_PLATFORM_ERROR
			)
	install(SCRIPT ${deploy_script})
endif (WIN32)

#-------------------------------------------------------------------------------
#	INSTALLER
#-------------------------------------------------------------------------------

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "Minesweeper")
set(CPACK_PACKAGE_VENDOR "Menari Softworks")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Free Qt/C++17 Minesweeper")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/icons/logo.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/resources/icons/logo.ico")
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/minesweeper.exe")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/nholthaus/minesweeper")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGE_EXECUTABLES "minesweeper" "minesweeper")
set(CPACK_CREATE_DESKTOP_LINKS minesweeper)
set(CPACK_PACKAGE_VERSION ${GIT_TAG})

include(CPack)