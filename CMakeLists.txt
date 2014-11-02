cmake_minimum_required(VERSION 2.8.9)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BUILD_DIR}" IS_IN_SOURCE_BUILD)
if(IS_IN_SOURCE_BUILD)
	message(AUTHOR_WARNING "You are building QChatCoreClient in-source. This is NOT recommended!")
endif()

if(WIN32)
	# In Qt 5.1+ we have our own main() function, don't autolink to qtmain on Windows
	cmake_policy(SET CMP0020 OLD)
endif()

project(QChatCoreClient)
enable_testing()

######## Set CMake options ########
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

######## Set module path ########
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
set(QCCC_SRC "${PROJECT_SOURCE_DIR}")
set(QCCC_BIN "${PROJECT_BINARY_DIR}")

# Output all executables and shared libs in the main build folder, not in subfolders.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

if(UNIX)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
endif()

######## Set compiler flags ########
include(UseCXX11)
set(CMAKE_CXX_FLAGS " -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror=return-type")

find_package(Qt5Core REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

include_directories(
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Concurrent_INCLUDE_DIRS}
	${Qt5Test_INCLUDE_DIRS}
	${Qt5Network_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
)


# The Qt5 cmake files don't provide its install paths, so ask qmake.
include(QMakeQuery)
query_qmake(QT_INSTALL_PLUGINS QT_PLUGINS_DIR)
query_qmake(QT_INSTALL_IMPORTS QT_IMPORTS_DIR)
query_qmake(QT_INSTALL_LIBS QT_LIBS_DIR)
query_qmake(QT_HOST_DATA QT_DATA_DIR)
set(QT_MKSPECS_DIR ${QT_DATA_DIR}/mkspecs)

######## Check endianness ########
include(TestBigEndian)
test_big_endian(BIGENDIAN)
if(${BIGENDIAN})
	add_definitions(-DCHATCORETESTBIG_ENDIAN)
endif(${BIGENDIAN})

set(QCCC_VERSION_MAJOR  0)
set(QCCC_VERSION_MINOR  0)
set(QCCC_VERSION_HOTFIX 0)

# Build number
set(QCCC_VERSION_BUILD -1 CACHE STRING "Build number. -1 for no build number.")

set(QCCC_RELEASE_VERSION_NAME "${QCCC_VERSION_MAJOR}.${QCCC_VERSION_MINOR}")
if(QCCC_VERSION_HOTFIX GREATER 0)
	set(QCCC_RELEASE_VERSION_NAME "${QCCC_RELEASE_VERSION_NAME}.${QCCC_VERSION_HOTFIX}")
endif()

if(QCCC_VERSION_BUILD GREATER 0)
    set(QCCC_VERSION_STRING "${QCCC_RELEASE_VERSION_NAME}-${QCCC_VERSION_BUILD}")
else()
    set(QCCC_VERSION_STRING "${QCCC_RELEASE_VERSION_NAME}")
endif()

#### Custom target to just print the version.
add_custom_target(version echo "Version: ${QCCC_VERSION_STRING}")

#### Check the current Git commit
include(GitFunctions)
git_run(COMMAND rev-parse HEAD DEFAULT "Unknown" OUTPUT_VAR QCCC_GIT_COMMIT)
message(STATUS "Git commit: ${QCCC_GIT_COMMIT}")

######## Configure header ########
configure_file("${PROJECT_SOURCE_DIR}/BuildConfig.cpp.in" "${PROJECT_BINARY_DIR}/BuildConfig.cpp")

# directories to look for dependencies
set(DIRS "${QT_LIBS_DIR}")

######## Packaging/install paths setup ########

if(UNIX AND APPLE)
	set(BINARY_DEST_DIR QCCC.app/Contents/MacOS)
	set(PLUGIN_DEST_DIR QCCC.app/Contents/MacOS)
	set(QTCONF_DEST_DIR QCCC.app/Contents/Resources)
	set(APPS "\${CMAKE_INSTALL_PREFIX}/QCCC.app")

	set(MACOSX_BUNDLE_BUNDLE_NAME "QCCC")
	set(MACOSX_BUNDLE_INFO_STRING "QCCC Minecraft launcher and management utility.")
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.QCCC.QCCC5")
	set(MACOSX_BUNDLE_BUNDLE_VERSION "${QCCC_VERSION_MAJOR}.${QCCC_VERSION_MINOR}.${QCCC_VERSION_REV}.${QCCC_VERSION_BUILD}")
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${QCCC_VERSION_MAJOR}.${QCCC_VERSION_MINOR}.${QCCC_VERSION_REV}.${QCCC_VERSION_BUILD}")
	set(MACOSX_BUNDLE_LONG_VERSION_STRING "${QCCC_VERSION_MAJOR}.${QCCC_VERSION_MINOR}.${QCCC_VERSION_REV}.${QCCC_VERSION_BUILD}")
	set(MACOSX_BUNDLE_ICON_FILE QCCC.icns)
	set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2013 QCCC Contributors")
elseif(UNIX)
	set(BINARY_DEST_DIR bin)
	set(PLUGIN_DEST_DIR plugins)
	set(QTCONF_DEST_DIR .)
	set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/QCCC")
elseif(WIN32)
	set(BINARY_DEST_DIR .)
	set(PLUGIN_DEST_DIR .)
	set(QTCONF_DEST_DIR .)
	set(APPS "\${CMAKE_INSTALL_PREFIX}/QCCC.exe")
endif()

# directories to look for dependencies
set(DIRS "${QT_LIBS_DIR}")

################################ Included Libs ################################

# Add the util library.
add_definitions(-DLIBUTIL_STATIC)
add_subdirectory(depends/util)
include_directories(${LIBUTIL_INCLUDE_DIR})

################################ FILES ################################

######## Sources and headers ########

set(SOURCE_FILES
    QCCC.cpp
    QCCC.h
    Utils.h
    BuildConfig.h
    ${PROJECT_BINARY_DIR}/BuildConfig.cpp

    QCCCError.h
    QCCCError.cpp
    logic/QCCCJson.h
    logic/QCCCJson.cpp

    logic/ChatCoreConnection.h
    logic/ChatCoreConnection.cpp
    logic/ChatCoreTransport.h
    logic/ChatCoreTransport.cpp

    logic/JsonTransport.h
    logic/JsonTransport.cpp
    
    logic/ChatCoreModel.h
    logic/ChatCoreModel.cpp
    logic/BufferModel.h
    logic/BufferModel.cpp

    gui/Platform.h
    gui/MainWindow.h
    gui/MainWindow.cpp
    gui/CoreConnectDialog.h
    gui/CoreConnectDialog.cpp
)

set(SOURCE_UIS
    gui/MainWindow.ui
    gui/CoreConnectDialog.ui
)

set(SOURCE_QRCS
    resources/qchatcoreclient/qccc.qrc
)

set(FILES_TO_TRANSLATE)
foreach(file ${SOURCE_FILES})
        get_filename_component(absfile "${file}" ABSOLUTE)
        list(APPEND FILES_TO_TRANSLATE "${absfile}")
endforeach()

foreach(file ${SOURCE_UIS})
        get_filename_component(absfile "${file}" ABSOLUTE)
        list(APPEND FILES_TO_TRANSLATE "${absfile}")
endforeach()

######## Windows resource files ########
if(WIN32)
        set(QCCC_RCS resources/qchatcoreclient.rc)
endif()

####### X11 Stuff #######
if(UNIX AND NOT APPLE)
	set(QCCC_QT_ADDITIONAL_MODULES ${QCCC_QT_ADDITIONAL_MODULES} X11Extras)
	set(QCCC_LINK_ADDITIONAL_LIBS ${QCCC_LINK_ADDITIONAL_LIBS} xcb)
	list(APPEND SOURCE_FILES gui/Platform_X11.cpp)
else()
	list(APPEND SOURCE_FILES gui/Platform_Other.cpp)
endif()


################################ FORMAT #################################

add_custom_target(Format)

foreach(file IN LISTS ${SOURCE_FILES})
    get_filename_component(dir "${file}" DIRECTORY)
    add_custom_command(TARGET Format
        COMMAND clang-format -style=file -i ${file}
        WORKING_DIRECTORY ${dir}
    )
endforeach()

################################ COMPILE ################################

# Qt 5 stuff
qt5_wrap_ui(QCCC_UI ${SOURCE_UIS})
qt5_add_resources(QCCC_RESOURCES ${SOURCE_QRCS})

# Link additional libraries
if(WIN32)
	set(QChatCoreClient_LINK_ADDITIONAL_LIBS ${QChatCoreClient_LINK_ADDITIONAL_LIBS} Qt5::WinMain)
endif(WIN32)

add_library(QChatCoreClient_common STATIC ${SOURCE_FILES} ${QCCC_UI} ${QCCC_RESOURCES})

add_executable(QChatCoreClient MACOSX_BUNDLE WIN32 main.cpp)

target_link_libraries(QChatCoreClient QChatCoreClient_common)
target_link_libraries(QChatCoreClient_common libUtil ${QChatCoreClient_LINK_ADDITIONAL_LIBS})

qt5_use_modules(QChatCoreClient Core Widgets Concurrent Network ${QCCC_QT_ADDITIONAL_MODULES})
qt5_use_modules(QChatCoreClient_common Core Widgets Concurrent Network ${QCCC_QT_ADDITIONAL_MODULES})

################################ INSTALLATION AND PACKAGING ################################

######## Install ########

#### Executable ####
if(APPLE AND UNIX) ## OSX
	install(TARGETS QChatCoreClient
		BUNDLE DESTINATION . COMPONENT Runtime
		RUNTIME DESTINATION QChatCoreClient.app/Contents/MacOS COMPONENT Runtime
	)

elseif(UNIX) ## LINUX and similar
	install(TARGETS QChatCoreClient
		BUNDLE DESTINATION . COMPONENT Runtime
		RUNTIME DESTINATION bin COMPONENT Runtime
	)
	install(PROGRAMS package/linux/QChatCoreClient DESTINATION .)

elseif(WIN32) ## WINDOWS
	install(TARGETS QChatCoreClient
		BUNDLE DESTINATION . COMPONENT Runtime
		LIBRARY DESTINATION . COMPONENT Runtime
		RUNTIME DESTINATION . COMPONENT Runtime
	)
endif()

#### Dist package logic ####

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	# Image formats
	install(
		DIRECTORY "${QT_PLUGINS_DIR}/imageformats"
		DESTINATION ${PLUGIN_DEST_DIR}
		COMPONENT Runtime
		REGEX "tga|tiff|mng" EXCLUDE
	)
	# Icon engines
	install(
		DIRECTORY "${QT_PLUGINS_DIR}/iconengines"
		DESTINATION ${PLUGIN_DEST_DIR}
		COMPONENT Runtime
		REGEX "fontawesome" EXCLUDE
	)
	# Platform plugins
	install(
		DIRECTORY "${QT_PLUGINS_DIR}/platforms"
		DESTINATION ${PLUGIN_DEST_DIR}
		COMPONENT Runtime
		REGEX "minimal|linuxfb|offscreen" EXCLUDE
	)
else()
	# Image formats
	install(
		DIRECTORY "${QT_PLUGINS_DIR}/imageformats"
		DESTINATION ${PLUGIN_DEST_DIR}
		COMPONENT Runtime
		REGEX "tga|tiff|mng" EXCLUDE
		REGEX "d\\." EXCLUDE
		REGEX "_debug\\." EXCLUDE
	)
	# Icon engines
	install(
		DIRECTORY "${QT_PLUGINS_DIR}/iconengines"
		DESTINATION ${PLUGIN_DEST_DIR}
		COMPONENT Runtime
		REGEX "fontawesome" EXCLUDE
		REGEX "d\\." EXCLUDE
		REGEX "_debug\\." EXCLUDE
	)
	# Platform plugins
	install(
		DIRECTORY "${QT_PLUGINS_DIR}/platforms"
		DESTINATION ${PLUGIN_DEST_DIR}
		COMPONENT Runtime
		REGEX "minimal|linuxfb|offscreen" EXCLUDE
		REGEX "d\\." EXCLUDE
		REGEX "_debug\\." EXCLUDE
	)
	if(APPLE)
	# Accessible plugin to make buttons look decent on osx
	install(
		DIRECTORY "${QT_PLUGINS_DIR}/accessible"
		DESTINATION ${PLUGIN_DEST_DIR}
		COMPONENT Runtime
		REGEX "quick" EXCLUDE
		REGEX "d\\." EXCLUDE
		REGEX "_debug\\." EXCLUDE
	)
	endif()
endif()

# qtconf
install(
	CODE "
file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${QTCONF_DEST_DIR}/qt.conf\" \"\")
"
	COMPONENT Runtime
)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/install_prereqs.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/install_prereqs.cmake"
	@ONLY
)
install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/install_prereqs.cmake" COMPONENT Runtime)

######## Package ########

# Package with CPack
if(UNIX)
	if(APPLE)
	set(CPACK_GENERATOR "ZIP")
	else()
	set(CPACK_GENERATOR "TGZ")
	endif()
elseif(WIN32)
	set(CPACK_GENERATOR "ZIP")
endif()
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)

set(CPACK_PACKAGE_NAME "QChatCoreClient 5")
set(CPACK_PACKAGE_VENDOR "")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "QCCC - Minecraft server management tool.")
set(CPACK_PACKAGE_VERSION "${QCCC_VERSION_MAJOR}.${QCCC_VERSION_MINOR}.${QCCC_VERSION_REV}.${QCCC_VERSION_BUILD}")
set(CPACK_PACKAGE_VERSION_MAJOR ${QCCC_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${QCCC_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${QCCC_VERSION_REV})

if(CPACK_GENERATOR STREQUAL "NSIS")
	set(CPACK_PACKAGE_FILE_NAME "Setup-QCCC")
else()
	set(CPACK_PACKAGE_FILE_NAME "QCCC")
endif()

if(WIN32)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "QCCC 5")
endif()

include(CPack)

# Translations
add_subdirectory(translations)
