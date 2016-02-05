cmake_minimum_required(VERSION 2.8.7)
project(exlauncher C CXX)

add_definitions(-std=c++11)

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
	ExLauncher/graphics_utils.cpp
	ExLauncher/main.cpp
	ExLauncher/App.cpp
	ExLauncher/DataStore.cpp
	ExLauncher/Screens/ScreenAppLaunch.cpp
	ExLauncher/Screens/ScreenTest.cpp
	ExLauncher/Screens/ScreenMenu.cpp
	ExLauncher/Screens/ScreenError.cpp
	ExLauncher/Screens/ScreenProgressDialog.cpp
	ExLauncher/Screens/ScreenMessageDialog.cpp
	ExLauncher/Screens/ScreenSystemDialog.cpp
	ExLauncher/Screens/ScreenBackgroundImage.cpp
	ExLauncher/utils.cpp
	ExLauncher/Animator.cpp
	ExLauncher/ThemeManager.cpp
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
	ExLauncher/AppManager.cpp
	ExLauncher/HomeDirectory.cpp
	ExLauncher/ScreenSystem/ScreenManager.cpp
	ExLauncher/ScreenSystem/Screen.cpp
	ExLauncher/ScreenSystem/InputState.cpp
)

include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${CPPFILES})

target_link_libraries(${PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_IMAGE}
	${SDL2_TTF}
	${SDL2_MIXER}
)
