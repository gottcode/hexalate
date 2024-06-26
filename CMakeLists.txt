# SPDX-FileCopyrightText: 2021-2023 Graeme Gott <graeme@gottcode.org>
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

# Configure project
project(hexalate VERSION 1.2.3 LANGUAGES CXX)

set(project_copyright "2009-2024 Graeme Gott")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui LinguistTools Widgets)
include(GNUInstallDirs)

add_compile_definitions(
	QT_NO_KEYWORDS
	$<$<CONFIG:DEBUG>:QT_STRICT_ITERATORS>
	$<$<CONFIG:DEBUG>:QT_NO_NARROWING_CONVERSIONS_IN_CONNECT>
	$<$<CONFIG:DEBUG>:QT_DISABLE_DEPRECATED_BEFORE=0x060700>
)

# Version number
include(cmake/AddVersionCompileDefinition.cmake)
add_version_compile_definition(src/main.cpp VERSIONSTR)

# Create program
qt_add_executable(hexalate
	src/board.cpp
	src/locale_dialog.cpp
	src/main.cpp
	src/piece.cpp
	src/puzzle.cpp
	src/window.cpp
	${translations_QM}
)

target_link_libraries(hexalate PRIVATE
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
)

# Create translations
file(GLOB translations_SRCS translations/*.ts)
qt_add_translations(hexalate
	TS_FILES ${translations_SRCS}
	QM_FILES_OUTPUT_VARIABLE translations_QM
	LUPDATE_OPTIONS -no-obsolete -locations none
)

# Optimize build
option(ENABLE_LINK_TIME_OPTIMIZATION "Enable link time optimization" OFF)
if(ENABLE_LINK_TIME_OPTIMIZATION)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT result)
	if(result)
		set_target_properties(hexalate PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
endif()

option(ENABLE_STRIP "Enable automatic stripping of builds" OFF)
if(ENABLE_STRIP)
	add_custom_command(TARGET hexalate
		POST_BUILD
		COMMAND ${CMAKE_STRIP} $<TARGET_FILE:hexalate>
	)
endif()

# Install
if(APPLE)
	set(datadir "../Resources")

	set_target_properties(hexalate PROPERTIES
		OUTPUT_NAME Hexalate
		MACOSX_BUNDLE TRUE
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/mac/Info.plist.in
	)

	include(cmake/BundleResources.cmake)
	bundle_data(hexalate ${CMAKE_SOURCE_DIR}/icons/hexalate.icns Resources)
	bundle_translations(hexalate "${translations_QM}")
elseif(WIN32)
	set(datadir ".")

	# Use Qt6 macro until CMake provides something
	# https://bugreports.qt.io/browse/QTBUG-87618
	set_target_properties(hexalate PROPERTIES
		OUTPUT_NAME Hexalate
		WIN32_EXECUTABLE TRUE
		QT_TARGET_VERSION "${PROJECT_VERSION}"
		QT_TARGET_COMPANY_NAME "Graeme Gott"
		QT_TARGET_DESCRIPTION "Color matching game"
		QT_TARGET_COPYRIGHT "\\xA9 ${project_copyright}"
		QT_TARGET_PRODUCT_NAME "Hexalate"
		QT_TARGET_RC_ICONS ${CMAKE_SOURCE_DIR}/icons/hexalate.ico
	)
	_qt_internal_generate_win32_rc_file(hexalate)
else()
	file(RELATIVE_PATH datadir ${CMAKE_INSTALL_FULL_BINDIR} ${CMAKE_INSTALL_FULL_DATADIR}/hexalate)

	target_sources(hexalate PRIVATE icons/icon.qrc)

	install(TARGETS hexalate RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
	install(FILES ${translations_QM} DESTINATION ${CMAKE_INSTALL_DATADIR}/hexalate/translations)
	install(FILES doc/hexalate.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6 COMPONENT doc)
	install(DIRECTORY icons/hicolor DESTINATION ${CMAKE_INSTALL_DATADIR}/icons)
	install(FILES icons/hexalate.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
	install(FILES icons/hexalate.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
endif()

set_property(SOURCE src/main.cpp APPEND PROPERTY COMPILE_DEFINITIONS HEXALATE_DATADIR="${datadir}")
