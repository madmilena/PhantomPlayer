cmake_minimum_required(VERSION 3.16)
project(PhantomPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Configuração do Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

# --- CORREÇÃO AQUI ---
# Ajustamos o caminho para apontar para a pasta específica onde
# o arquivo SFMLConfig.cmake está localizado.
set(CMAKE_PREFIX_PATH "/Users/milenamadsen/libs/SFML-3.0.0-macOS-clang-64-bit")
find_package(SFML 3.0 REQUIRED COMPONENTS Audio)

# Configuração do TagLib
find_package(TagLib REQUIRED)

# Lista de todos os seus arquivos-fonte
set(EXECUTABLE_NAME phantom_player)
add_executable(phantom_player
        src/main.cpp
        src/core/AudioEngine.cpp
        src/core/MediaLibrary.cpp
        src/services/PlaybackService.cpp
        src/services/PlaylistManager.cpp
        src/ui/MainWindow.cpp
        src/ui/components/PlayerControlsWidget.cpp
        src/ui/components/SearchBarWidget.cpp
        src/ui/components/TrackDetailsComponent.cpp
        src/ui/tabs/BaseTab.cpp
        src/ui/tabs/LibraryTabWidget.cpp
        src/ui/tabs/PlaylistTabWidget.cpp
        ${RESOURCES}
        src/ui/components/PlaylistComponent.cpp
        src/ui/components/PlayerInfoWidget.h
        src/ui/components/PlayerInfoWidget.cpp
        src/ui/components/PlayerControlsComponent.h
        src/ui/components/PlayerControlsComponent.cpp
        src/services/SearchService.h
        src/services/SearchService.cpp
)

# Diretórios de inclusão
target_include_directories(phantom_player PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
        ${TAGLIB_INCLUDE_DIRS}
)

# Bibliotecas para linkar
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        Qt6::Widgets
        Qt6::Network
        SFML::Audio
        ${TAGLIB_LIBRARY_FILE}
        sfml-audio
        sfml-system
        TagLib::tag
)