cmake_minimum_required(VERSION 3.16)
project(PhantomPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "/usr/local/opt/qt")
find_package(Qt6 COMPONENTS Widgets Gui REQUIRED)

set(CMAKE_PREFIX_PATH "/Users/milenamadsen/libs/SFML-3.0.0-macOS-clang-64-bit")
find_package(SFML 3.0 REQUIRED COMPONENTS Audio)

find_package(TagLib REQUIRED)
find_library(TAGLIB_LIBRARY_FILE NAMES tag)

qt_add_resources(RESOURCES resources/resources.qrc)

add_executable(phantom_player
    src/main.cpp
    src/core/AudioEngine.cpp
    src/core/MediaLibrary.cpp
    src/services/PlaybackService.cpp
    src/services/PlaylistManager.cpp
    
    # --- UI ---
    src/ui/MainWindow.cpp
    
    # --- Componentes da UI ---
    src/ui/components/PlayerControlsWidget.cpp
    src/ui/components/SearchBarWidget.cpp
    src/ui/components/TrackDetailsComponent.cpp
    
    # --- Abas da UI ---
    src/ui/tabs/BaseTab.cpp
    src/ui/tabs/LibraryTabWidget.cpp
    src/ui/tabs/PlaylistTabWidget.cpp
    
    ${RESOURCES}
)

target_include_directories(phantom_player PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(phantom_player PRIVATE
    Qt6::Widgets
    Qt6::Gui
    SFML::Audio
    ${TAGLIB_LIBRARY_FILE}
)