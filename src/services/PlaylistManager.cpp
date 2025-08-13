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

// --- ESQUELETO DAS FUNÇÕES PARA O LINKER FUNCIONAR ---
// (Sua lógica do desafio virá aqui depois)

bool PlaylistManager::savePlaylistsToFile(const QString& filePath) {
    std::cout << "Lógica de salvar em: " << filePath.toStdString() << " ainda nao implementada." << std::endl;
    // SUA LÓGICA DE SALVAR O JSON VAI AQUI
    return true;
}

bool PlaylistManager::loadPlaylistsFromFile(const QString& filePath) {
    std::cout << "Lógica de carregar de: " << filePath.toStdString() << " ainda nao implementada." << std::endl;
    // SUA LÓGICA DE CARREGAR O JSON VAI AQUI
    emit playlistsChanged(); // Lembrar de emitir o sinal no final
    return true;
}