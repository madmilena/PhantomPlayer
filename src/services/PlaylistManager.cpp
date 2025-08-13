#include "PlaylistManager.h"
#include <fstream>
#include <iostream>
#include <QStandardPaths>
#include <QDir>
#include "nlohmann/json.hpp" // Inclui a biblioteca de JSON

using json = nlohmann::json;

PlaylistManager::PlaylistManager(MediaLibrary* mediaLibrary, QObject* parent)
    : QObject(parent), m_mediaLibrary(mediaLibrary) {
    
    // Opcional: Se quiser que as playlists carreguem automaticamente ao iniciar
    // QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // if (!dataPath.isEmpty()) {
    //     QDir dir(dataPath);
    //     if (!dir.exists()) dir.mkpath(".");
    //     QString filePath = dataPath + "/playlists.json";
    //     loadPlaylistsFromFile(filePath);
    // }
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
        emit playlistsChanged(); // Avisa a UI para redesenhar a aba da playlist
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
    json j = json::array();
    const auto& allTracks = m_mediaLibrary->getTracks();

    for (const auto& playlist : m_playlists) {
        json playlistObject;
        playlistObject["name"] = playlist.name.toStdString();
        
        json trackPaths = json::array();
        for (int trackIndex : playlist.trackIndices) {
            if (trackIndex >= 0 && trackIndex < allTracks.size()) {
                trackPaths.push_back(allTracks[trackIndex].filePath);
            }
        }
        playlistObject["tracks"] = trackPaths;
        j.push_back(playlistObject);
    }

    std::ofstream file(filePath.toStdString());
    if (file.is_open()) {
        file << j.dump(4);
        return true;
    }
    
    std::cerr << "Erro: Nao foi possivel abrir o arquivo para escrita: " << filePath.toStdString() << std::endl;
    return false;
}

bool PlaylistManager::loadPlaylistsFromFile(const QString& filePath) {
    std::ifstream file(filePath.toStdString());
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo para leitura: " << filePath.toStdString() << std::endl;
        return false;
    }

    try {
        json j = json::parse(file);

        m_playlists.clear();
        const auto& allTracks = m_mediaLibrary->getTracks();
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
                    if (trackPathToIndex.count(pathStr)) {
                        newPlaylist.trackIndices.push_back(trackPathToIndex[pathStr]);
                    }
                }
            }
            // A CORREÇÃO ESTÁ AQUI:
            m_playlists.push_back(newPlaylist);
        }

        emit playlistsChanged();
        return true;

    } catch (const json::parse_error& e) {
        std::cerr << "Erro ao decodificar o arquivo de playlists JSON: " << e.what() << std::endl;
        return false;
    }
}