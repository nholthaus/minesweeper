set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0087 NEW)
configure_file(appinfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/appinfo.h @ONLY NEWLINE_STYLE UNIX)
configure_file(${CMAKE_SOURCE_DIR}/resources/linux/minesweeper.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/minesweeper.desktop @ONLY NEWLINE_STYLE UNIX)

if(APPLE)
	set(MACOS_ICONSET_DIR ${CMAKE_CURRENT_BINARY_DIR}/minesweeper.iconset)
	set(MACOS_BUNDLE_ICON ${CMAKE_CURRENT_BINARY_DIR}/minesweeper.icns)
	add_custom_command(
		OUTPUT ${MACOS_BUNDLE_ICON}
		COMMAND ${CMAKE_COMMAND} -E rm -rf ${MACOS_ICONSET_DIR}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${MACOS_ICONSET_DIR}
		COMMAND sips -z 16 16 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_16x16.png
		COMMAND sips -z 32 32 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_16x16@2x.png
		COMMAND sips -z 32 32 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_32x32.png
		COMMAND sips -z 64 64 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_32x32@2x.png
		COMMAND sips -z 128 128 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_128x128.png
		COMMAND sips -z 256 256 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_128x128@2x.png
		COMMAND sips -z 256 256 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_256x256.png
		COMMAND sips -z 512 512 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_256x256@2x.png
		COMMAND sips -z 512 512 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_512x512.png
		COMMAND sips -z 1024 1024 ${CMAKE_SOURCE_DIR}/resources/images/mine.png --out ${MACOS_ICONSET_DIR}/icon_512x512@2x.png
		COMMAND iconutil -c icns ${MACOS_ICONSET_DIR} -o ${MACOS_BUNDLE_ICON}
		DEPENDS ${CMAKE_SOURCE_DIR}/resources/images/mine.png
		VERBATIM
		)
endif()

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
                   $<$<PLATFORM_ID:Darwin>:${MACOS_BUNDLE_ICON}>
                   )

if(APPLE)
	set_source_files_properties(${MACOS_BUNDLE_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

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
	                      MACOSX_BUNDLE_SHORT_VERSION_STRING "${GIT_TAG}"
	                      MACOSX_BUNDLE_GUI_IDENTIFIER "github.com/nholthaus/minesweeper"
	                      MACOSX_BUNDLE_ICON_FILE "minesweeper.icns"
	                      )
	# Install the .app bundle
	install(TARGETS ${PROJECT_NAME}
	        BUNDLE DESTINATION .
	        COMPONENT Runtime)
else ()
	set_target_properties(${PROJECT_NAME} PROPERTIES
	                      INSTALL_RPATH ""
	                      INSTALL_RPATH_USE_LINK_PATH FALSE
	                      )
	install(TARGETS ${PROJECT_NAME}
	        RUNTIME DESTINATION bin
	        )
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/minesweeper.desktop
	        DESTINATION share/applications
	        )
	install(FILES ${CMAKE_SOURCE_DIR}/resources/images/mine.png
	        DESTINATION share/icons/hicolor/256x256/apps
	        RENAME minesweeper.png
	        )
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
elseif (APPLE)
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

set(CPACK_PACKAGE_NAME "Minesweeper")
set(CPACK_PACKAGE_VENDOR "Menari Softworks")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Free Qt/C++17 Minesweeper")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/nholthaus/minesweeper")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGE_EXECUTABLES "minesweeper" "minesweeper")
set(CPACK_CREATE_DESKTOP_LINKS minesweeper)
set(CPACK_PACKAGE_VERSION ${GIT_TAG})

if (WIN32)
	set(CPACK_GENERATOR NSIS)
	set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/icons/logo.ico")
	set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/resources/icons/logo.ico")
	set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/minesweeper.exe")
elseif(UNIX AND NOT APPLE)
	set(CPACK_GENERATOR DEB)
	set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nic Holthaus")
	set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
	set(CPACK_DEBIAN_PACKAGE_SECTION "games")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
endif()

include(CPack)
