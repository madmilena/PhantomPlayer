#include "PlaylistManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

// Construtor correto
PlaylistManager::PlaylistManager(QObject *parent) : QObject(parent) {}

void PlaylistManager::createNewPlaylist(const QString& name) {
    if (name.isEmpty()) return;
    m_playlists.emplace_back(name.toStdString());
    emit playlistsChanged();
}

void PlaylistManager::addTrackToPlaylist(const int playlistIndex, const int trackId) {
    if (playlistIndex >= 0 && playlistIndex < m_playlists.size()) {
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
        m_playlists[playlistIndex].removeTrack(trackIndex);
        emit playlistsChanged();
    }
}

// --- Definição Corrigida (com 'const' e retornando 'bool') ---
bool PlaylistManager::savePlaylistsToFile(const QString& filePath) const {
    QJsonArray playlistsArray;
    for (const auto& playlist : m_playlists) {
        QJsonObject playlistObject;
        playlistObject["name"] = QString::fromStdString(playlist.getName());

        QJsonArray trackIdsArray;
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
        return true; // Sucesso
    }

    qDebug() << "Não foi possível salvar o arquivo de playlists:" << file.errorString();
    return false; // Falha
}

// --- Definição Corrigida (retornando 'bool') ---
bool PlaylistManager::loadPlaylistsFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Não foi possível abrir o arquivo de playlists:" << file.errorString();
        return false; // Falha
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray playlistsArray = doc.array();

    m_playlists.clear();

    for (const QJsonValue& value : playlistsArray) {
        QJsonObject playlistObject = value.toObject();
        QString name = playlistObject.value("name").toString("Nova Playlist");

        Playlist newPlaylist(name.toStdString());

        if (playlistObject.contains("tracks") && playlistObject["tracks"].isArray()) {
            QJsonArray tracksArray = playlistObject["tracks"].toArray();
            for (const QJsonValue& trackValue : tracksArray) {
                newPlaylist.addTrack(trackValue.toInt());
            }
        }
        m_playlists.push_back(newPlaylist);
    }

    emit playlistsChanged();
    return true; // Sucesso
}

const std::vector<Playlist>& PlaylistManager::getPlaylists() const {
    return m_playlists;
}