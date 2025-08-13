#include "PlaylistManager.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

PlaylistManager::PlaylistManager(MediaLibrary* mediaLibrary, QObject* parent)
    : QObject(parent), m_mediaLibrary(mediaLibrary) {
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

const std::vector<Playlist>& PlaylistManager::getPlaylists() const {
    return m_playlists;
}

bool PlaylistManager::savePlaylistsToFile(const QString& filePath) {
    std::cout << "Lógica de salvar em: " << filePath.toStdString() << " ainda nao implementada." << std::endl;
    // Sua lógica de salvar o JSON virá aqui.
    return true;
}

bool PlaylistManager::loadPlaylistsFromFile(const QString& filePath) {
    std::cout << "Lógica de carregar de: " << filePath.toStdString() << " ainda nao implementada." << std::endl;
    // Sua lógica de carregar o JSON virá aqui.
    emit playlistsChanged();
    return true;
}