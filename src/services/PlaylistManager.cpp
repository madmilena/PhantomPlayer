#include "PlaylistManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <unordered_map>

PlaylistManager::PlaylistManager(QObject *parent) : QObject(parent) {}

void PlaylistManager::createNewPlaylist(const QString& name) {
    if (name.isEmpty()) return;

    // --- CORREÇÃO ---
    // Chama o construtor de Playlist que espera apenas um nome.
    m_playlists.emplace_back(name.toStdString());

    qDebug() << "Playlist criada:" << name << ", emitindo sinal de mudança.";
    emit playlistsChanged();
}

void PlaylistManager::addTrackToPlaylist(const int playlistIndex, const int trackId) {
    if (playlistIndex >= 0 && playlistIndex < m_playlists.size()) {
        // --- CORREÇÃO ---
        // Usa o método público addTrack para modificar a lista de faixas.
        m_playlists[playlistIndex].addTrack(trackId);
        emit playlistsChanged();
    }
}

void PlaylistManager::deletePlaylist(const int playlistIndex) {
    if (playlistIndex >= 0 && playlistIndex < m_playlists.size()) {
        m_playlists.erase(m_playlists.begin() + playlistIndex);
        emit playlistsChanged();
    }
}

void PlaylistManager::removeTrackFromPlaylist(const int playlistIndex, const int trackIndex) {
    if (playlistIndex >= 0 && playlistIndex < m_playlists.size()) {
        // --- CORREÇÃO ---
        // Usa o método público removeTrack.
        m_playlists[playlistIndex].removeTrack(trackIndex);
        emit playlistsChanged();
    }
}

void PlaylistManager::savePlaylistsToFile(const QString& filePath) {
    QJsonArray playlistsArray;
    for (const auto& playlist : m_playlists) {
        QJsonObject playlistObject;
        // --- CORREÇÃO ---
        // Usa o getter getName() para ler o nome da playlist.
        playlistObject["name"] = QString::fromStdString(playlist.getName());

        QJsonArray trackIdsArray;
        // --- CORREÇÃO ---
        // Usa o getter getTrackIds() para ler os IDs das faixas.
        for (int trackId : playlist.getTrackIds()) {
            trackIdsArray.append(trackId);
        }
        playlistObject["tracks"] = trackIdsArray;
        playlistsArray.append(playlistObject);
    }

    QJsonDocument doc(playlistsArray);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "Não foi possível salvar o arquivo de playlists:" << file.errorString();
    }
}

void PlaylistManager::loadPlaylistsFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Não foi possível abrir o arquivo de playlists:" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray playlistsArray = doc.array();

    m_playlists.clear();

    for (const QJsonValue& value : playlistsArray) {
        QJsonObject playlistObject = value.toObject();
        QString name = playlistObject.value("name").toString("Nova Playlist");

        // --- CORREÇÃO ---
        // Cria a playlist usando o construtor correto.
        Playlist newPlaylist(name.toStdString());

        if (playlistObject.contains("tracks") && playlistObject["tracks"].isArray()) {
            QJsonArray tracksArray = playlistObject["tracks"].toArray();
            for (const QJsonValue& trackValue : tracksArray) {
                // Adiciona as faixas usando o método público.
                newPlaylist.addTrack(trackValue.toInt());
            }
        }
        m_playlists.push_back(newPlaylist);
    }

    emit playlistsChanged();
}

const std::vector<Playlist>& PlaylistManager::getPlaylists() const {
    return m_playlists;
}