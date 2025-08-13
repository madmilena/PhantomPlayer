#include "PlaylistManager.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

PlaylistManager::PlaylistManager(MediaLibrary* mediaLibrary, QObject* parent)
    : QObject(parent), m_mediaLibrary(mediaLibrary) {
    // O construtor agora está limpo. O carregamento é manual, via menu.
}

void PlaylistManager::createNewPlaylist(const QString& name) {
    if (!name.isEmpty()) {
        m_playlists.emplace_back(Playlist{name, {}});
        emit playlistsChanged();
    }
}

void PlaylistManager::addTrackToPlaylist(int playlistIndex, int trackIndex) {
    if (playlistIndex >= 0 && playlistIndex < m_playlists.size()) {
        m_playlists[playlistIndex].trackIndices.push_back(trackIndex);
        emit playlistsChanged();
    }
}

void PlaylistManager::deletePlaylist(int playlistIndex) {
    if (playlistIndex >= 0 && playlistIndex < m_playlists.size()) {
        m_playlists.erase(m_playlists.begin() + playlistIndex);
        emit playlistsChanged();
    }
}

const std::vector<Playlist>& PlaylistManager::getPlaylists() const {
    return m_playlists;
}

bool PlaylistManager::savePlaylistsToFile(const QString& filePath) {
    // Implemente a lógica JSON aqui
    std::cout << "Salvando playlists em: " << filePath.toStdString() << std::endl;
    return true;
}

bool PlaylistManager::loadPlaylistsFromFile(const QString& filePath) {
    // Implemente a lógica JSON aqui
    std::cout << "Carregando playlists de: " << filePath.toStdString() << std::endl;
    emit playlistsChanged();
    return true;
}