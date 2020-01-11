cmake_minimum_required(VERSION 2.8.7)
project(exlauncher C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(FindSDL2 REQUIRED)

find_library(SDL2_TTF SDL2_ttf)
find_library(SDL2_IMAGE SDL2_image)
find_library(SDL2_MIXER SDL2_mixer)

if (NOT SDL2_TTF)
	message(SEND_ERROR "SDL2_ttf is required to build this software.")
endif()
if (NOT SDL2_IMAGE)
	message(SEND_ERROR "SDL2_image is required to build this software.")
endif()
if (NOT SDL2_MIXER)
	message(SEND_ERROR "SDL2_mixer is required to build this software.")
endif()

set(CPPFILES
	ExLauncher/ResourceManager.cpp
	ExLauncher/Graphics/graphics_utils.cpp
	ExLauncher/Graphics/Graphics.cpp
	ExLauncher/main.cpp
	ExLauncher/App/App.cpp
	ExLauncher/App/RecentApp.cpp
	ExLauncher/ScreenSystem/DataStore.cpp
	ExLauncher/Screens/ScreenAppLaunch.cpp
	ExLauncher/Screens/ScreenMenu.cpp
	ExLauncher/Screens/ScreenError.cpp
	ExLauncher/Screens/ScreenProgressDialog.cpp
	ExLauncher/Screens/ScreenMessageDialog.cpp
	ExLauncher/Screens/ScreenSystemDialog.cpp
	ExLauncher/Screens/ScreenBackgroundImage.cpp
	ExLauncher/Screens/ScreenSystemInfo.cpp
	ExLauncher/Screens/ScreenShutdown.cpp
	ExLauncher/Screens/ScreenSettingsSelectTheme.cpp
	ExLauncher/Screens/ScreenBrowseFilesForApp.cpp
	ExLauncher/utils.cpp
	ExLauncher/Animator.cpp
	ExLauncher/Theme/Theme.cpp
	ExLauncher/Theme/ThemeManager.cpp
	ExLauncher/Views/Image.cpp
	ExLauncher/Views/LinearPanel.cpp
	ExLauncher/Views/ListView.cpp
	ExLauncher/Views/State.cpp
	ExLauncher/Views/TabStrip.cpp
	ExLauncher/Views/FramePanel.cpp
	ExLauncher/Views/Label.cpp
	ExLauncher/Views/GridView.cpp
	ExLauncher/Views/Selector.cpp
	ExLauncher/Views/TabPanel.cpp
	ExLauncher/Views/RecentAppView.cpp
	ExLauncher/Views/RectangleShape.cpp
	ExLauncher/Xml.cpp
	ExLauncher/structures.cpp
	ExLauncher/ViewSystem/CollectionView.cpp
	ExLauncher/ViewSystem/ScrollView.cpp
	ExLauncher/ViewSystem/View.cpp
	ExLauncher/ViewSystem/LayoutHelper.cpp
	ExLauncher/App/AppManager.cpp
	ExLauncher/Filesystem/FilesystemUtils.cpp
	ExLauncher/Filesystem/FilesystemWatchListener.cpp
	ExLauncher/Filesystem/FilesystemWatchManager.cpp
	ExLauncher/Filesystem/HomeDirectory.cpp
	ExLauncher/Filesystem/SimpleFileWatcher/FileWatcher.cpp
	ExLauncher/Filesystem/SimpleFileWatcher/FileWatcherLinux.cpp
	ExLauncher/Filesystem/SimpleFileWatcher/FileWatcherOSX.cpp
	ExLauncher/Filesystem/SimpleFileWatcher/FileWatcherWin32.cpp
	ExLauncher/ScreenSystem/ScreenManager.cpp
	ExLauncher/ScreenSystem/Screen.cpp
	ExLauncher/ScreenSystem/InputState.cpp
	ExLauncher/Graphics/GaussianBlur.cpp
)

include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${CPPFILES})

target_link_libraries(${PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_IMAGE}
	${SDL2_TTF}
	${SDL2_MIXER}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
)

find_library(LIBOPK opk)
find_path(LIBOPK_INCLUDE_DIR opk.h)

if (LIBOPK AND LIBOPK_INCLUDE_DIR)
	option(ENABLE_OPK "Enable support for OPKs" ON)

	if (ENABLE_OPK)
		include_directories(${LIBOPK_INCLUDE_DIR})
		target_link_libraries(${PROJECT_NAME} ${LIBOPK})
		add_definitions(-DHAS_LIBOPK=1)
	endif()
endif()

find_library(LIBXDGMIME xdgmime)
find_path(LIBXDGMIME_INCLUDE_DIR xdgmime.h)

if (LIBXDGMIME AND LIBXDGMIME_INCLUDE_DIR)
	option(ENABLE_XDGMIME "Enable support for file browser filtering" ON)

	if (ENABLE_XDGMIME)
		include_directories(${LIBXDGMIME_INCLUDE_DIR})
		target_link_libraries(${PROJECT_NAME} ${LIBXDGMIME})
		add_definitions(-DHAS_LIBXDGMIME=1)
	endif()
endif()
