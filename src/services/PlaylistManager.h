#ifndef PHANTOMPLAYER_PLAYLISTMANAGER_H
#define PHANTOMPLAYER_PLAYLISTMANAGER_H

#include <QObject>
#include <vector>
#include <QString>
#include "core/Playlist.h"

class PlaylistManager : public QObject {
    Q_OBJECT

public:
    explicit PlaylistManager(QObject* parent = nullptr);

    void createNewPlaylist(const QString& name);
    void addTrackToPlaylist(int playlistIndex, int trackId);
    void deletePlaylist(int playlistIndex);
    void removeTrackFromPlaylist(int playlistIndex, int trackIndex);

    // --- Assinaturas Corretas ---
    // A função de salvar deve ser 'const' pois apenas lê os dados.
    [[nodiscard]] bool savePlaylistsToFile(const QString& filePath) const;
    // A função de carregar modifica o estado, então não é 'const' e retorna sucesso/falha.
    bool loadPlaylistsFromFile(const QString& filePath);

    [[nodiscard]] const std::vector<Playlist>& getPlaylists() const;

    signals:
        void playlistsChanged();

private:
    std::vector<Playlist> m_playlists;
};

#endif //PHANTOMPLAYER_PLAYLISTMANAGER_H