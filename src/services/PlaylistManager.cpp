#include "PlaylistManager.h"
#include <fstream>
#include <iostream>
#include <QStandardPaths>
#include <QDir>

// Inclui a biblioteca de JSON que baixamos
#include "nlohmann/json.hpp"

using json = nlohmann::json;

PlaylistManager::PlaylistManager(MediaLibrary* mediaLibrary, QObject* parent)
    : QObject(parent), m_mediaLibrary(mediaLibrary) {
    
    // Encontra o local padrão para dados de aplicativos
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataPath.isEmpty()) {
        dataPath = QDir::homePath() + "/.SpotifyClone"; // Fallback
    }
    
    // Cria o diretório se ele não existir
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    m_saveFilePath = dataPath + "/playlists.json";
    std::cout << "Arquivo de playlists sera salvo em: " << m_saveFilePath.toStdString() << std::endl;

    // Carrega as playlists existentes ao iniciar
    loadPlaylistsFromFile();
}

void PlaylistManager::createNewPlaylist(const QString& name) {
    if (!name.isEmpty()) {
        m_playlists.emplace_back(Playlist{name, {}});
        savePlaylistsToFile(); // Salva automaticamente
        emit playlistsChanged();
    }
}

void PlaylistManager::addTrackToPlaylist(int playlistIndex, int trackIndex) {
    if (playlistIndex >= 0 && playlistIndex < m_playlists.size()) {
        m_playlists[playlistIndex].trackIndices.push_back(trackIndex);
        savePlaylistsToFile(); // Salva automaticamente
        emit playlistsChanged();
    }
}

const std::vector<Playlist>& PlaylistManager::getPlaylists() const {
    return m_playlists;
}

void PlaylistManager::savePlaylistsToFile() const {
    json j = json::array(); // Cria um array JSON

    const auto& allTracks = m_mediaLibrary->getTracks();

    for (const auto& playlist : m_playlists) {
        json playlistObject;
        playlistObject["name"] = playlist.name.toStdString();
        playlistObject["tracks"] = json::array();

        for (int trackIndex : playlist.trackIndices) {
            if (trackIndex >= 0 && trackIndex < allTracks.size()) {
                // Salva o caminho completo do arquivo da música
                playlistObject["tracks"].push_back(allTracks[trackIndex].filePath);
            }
        }
        j.push_back(playlistObject);
    }

    // Abre o arquivo e escreve o JSON formatado
    std::ofstream file(m_saveFilePath.toStdString());
    if (file.is_open()) {
        file << j.dump(4); // .dump(4) formata o JSON para ser legível
    } else {
        std::cerr << "Erro ao abrir o arquivo de playlists para escrita." << std::endl;
    }
}

void PlaylistManager::loadPlaylistsFromFile() {
    std::ifstream file(m_saveFilePath.toStdString());
    if (!file.is_open()) {
        std::cout << "Arquivo de playlists nao encontrado. Comecando com lista vazia." << std::endl;
        return; // Normal na primeira vez que o app roda
    }

    try {
        json j = json::parse(file);

        m_playlists.clear();
        const auto& allTracks = m_mediaLibrary->getTracks();

        // Cria um mapa para busca rápida de filePath -> index (muito mais eficiente)
        std::map<std::string, int> trackPathToIndex;
        for (int i = 0; i < allTracks.size(); ++i) {
            trackPathToIndex[allTracks[i].filePath] = i;
        }

        for (const auto& playlistObject : j) {
            Playlist newPlaylist;
            newPlaylist.name = QString::fromStdString(playlistObject.value("name", ""));
            
            for (const auto& trackPath : playlistObject["tracks"]) {
                std::string pathStr = trackPath.get<std::string>();
                // Procura o caminho no nosso mapa para encontrar o índice
                if (trackPathToIndex.count(pathStr)) {
                    newPlaylist.trackIndices.push_back(trackPathToIndex[pathStr]);
                }
            }
            m_playlists.push_back(newPlaylist);
        }

        emit playlistsChanged(); // Notifica a UI que as playlists foram carregadas
    } catch (const json::parse_error& e) {
        std::cerr << "Erro ao decodificar o arquivo de playlists JSON: " << e.what() << std::endl;
    }
}