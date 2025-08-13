#ifndef SPOTIFYCLONE_PLAYLISTMANAGER_H
#define SPOTIFYCLONE_PLAYLISTMANAGER_H

#include <QObject>
#include "../core/Playlist.h"
#include "../core/MediaLibrary.h"

class PlaylistManager : public QObject {
    Q_OBJECT

public:
    explicit PlaylistManager(MediaLibrary* mediaLibrary, QObject* parent = nullptr);

    void createNewPlaylist(const QString& name);
    void addTrackToPlaylist(int playlistIndex, int trackIndex);
    const std::vector<Playlist>& getPlaylists() const;

signals:
    void playlistsChanged();

private:
    void savePlaylistsToFile() const;
    void loadPlaylistsFromFile();

    std::vector<Playlist> m_playlists;
    MediaLibrary* m_mediaLibrary;
    QString m_saveFilePath; // Caminho para o nosso arquivo JSON
};

#endif //SPOTIFYCLONE_PLAYLISTMANAGER_H