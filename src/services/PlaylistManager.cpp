#include "PlaylistManager.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp" // Inclui a biblioteca de JSON

// Conveniência para usar a biblioteca json
using json = nlohmann::json;

PlaylistManager::PlaylistManager(MediaLibrary* mediaLibrary, QObject* parent)
    : QObject(parent), m_mediaLibrary(mediaLibrary) {
    // O construtor está limpo, o carregamento/salvamento será manual via menu
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

// --- IMPLEMENTAÇÃO COMPLETA DO SALVAMENTO ---
bool PlaylistManager::savePlaylistsToFile(const QString& filePath) {
    json j = json::array(); // O arquivo JSON será um array de playlists

    const auto& allTracks = m_mediaLibrary->getTracks();

    for (const auto& playlist : m_playlists) {
        json playlistObject;
        playlistObject["name"] = playlist.name.toStdString();
        
        // Cria um array apenas com os caminhos dos arquivos de música
        json trackPaths = json::array();
        for (int trackIndex : playlist.trackIndices) {
            if (trackIndex >= 0 && trackIndex < allTracks.size()) {
                trackPaths.push_back(allTracks[trackIndex].filePath);
            }
        }
        playlistObject["tracks"] = trackPaths;
        j.push_back(playlistObject);
    }

    // Abre o arquivo e escreve o JSON formatado
    std::ofstream file(filePath.toStdString());
    if (file.is_open()) {
        file << j.dump(4); // .dump(4) formata o JSON com 4 espaços de indentação
        return true;
    }
    
    std::cerr << "Erro: Nao foi possivel abrir o arquivo para escrita: " << filePath.toStdString() << std::endl;
    return false;
}

// --- IMPLEMENTAÇÃO COMPLETA DO CARREGAMENTO ---
bool PlaylistManager::loadPlaylistsFromFile(const QString& filePath) {
    std::ifstream file(filePath.toStdString());
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo para leitura: " << filePath.toStdString() << std::endl;
        return false;
    }

    try {
        json j = json::parse(file);

        m_playlists.clear(); // Limpa as playlists atuais antes de carregar
        const auto& allTracks = m_mediaLibrary->getTracks();

        // Para eficiência, criamos um mapa para encontrar rapidamente o índice de uma música pelo seu caminho
        std::map<std::string, int> trackPathToIndex;
        for (int i = 0; i < allTracks.size(); ++i) {
            trackPathToIndex[allTracks[i].filePath] = i;
        }

        for (const auto& playlistObject : j) {
            Playlist newPlaylist;
            newPlaylist.name = QString::fromStdString(playlistObject.value("name", ""));
            
            if (playlistObject.contains("tracks") && playlistObject["tracks"].is_array()) {
                for (const auto& trackPath : playlistObject["tracks"]) {
                    std::string pathStr = trackPath.get<std::string>();
                    // Se o caminho da música do arquivo JSON existir na nossa biblioteca atual...
                    if (trackPathToIndex.count(pathStr)) {
                        // ...adicionamos seu índice à nova playlist.
                        newPlaylist.trackIndices.push_back(trackPathToIndex[pathStr]);
                    }
                }
            }
            m_playlists.push