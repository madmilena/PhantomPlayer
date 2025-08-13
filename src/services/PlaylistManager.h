#ifndef PHANTOMPLAYER_PLAYLISTMANAGER_H
#define PHANTOMPLAYER_PLAYLISTMANAGER_H

#include <QObject>
#include "core/Playlist.h"
#include "core/MediaLibrary.h"

class PlaylistManager : public QObject {
    Q_OBJECT

public:
    explicit PlaylistManager(MediaLibrary* mediaLibrary, QObject* parent = nullptr);

    void createNewPlaylist(const QString& name);
    void addTrackToPlaylist(int playlistIndex, int trackIndex);
    void deletePlaylist(int playlistIndex);
    const std::vector<Playlist>& getPlaylists() const;

    bool savePlaylistsToFile(const QString& filePath);
    bool loadPlaylistsFromFile(const QString& filePath);

signals:
    void playlistsChanged();

private:
    std::vector<Playlist> m_playlists;
    MediaLibrary* m_mediaLibrary;
};

#endif //PHANTOMPLAYER_PLAYLISTMANAGER_H