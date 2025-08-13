#ifndef PHANTOMPLAYER_PLAYLISTCOMPONENT_H
#define PHANTOMPLAYER_PLAYLISTCOMPONENT_H

#include <QWidget>
#include <QListWidget>
#include <vector>
#include "../../core/Track.h"
#include "../../core/Playlist.h" // supondo que tenha Playlist com id, nome e lista de Tracks

class PlaylistComponent final : public QWidget {
    Q_OBJECT
public:
    explicit PlaylistComponent(QWidget* parent = nullptr);

    void setPlaylists(const std::vector<Playlist>& playlists);
    void setCurrentPlaylist(int playlistId);
    void updateTracks(const std::vector<Track>& tracks);

signals:
    void trackDoubleClicked(int trackIndex);
    void playlistSelected(int playlistId);

private slots:
    void onPlaylistSelected(const QListWidgetItem* item);
    void onTrackDoubleClicked(const QListWidgetItem* item);

private:
    void refreshPlaylists() const;
    void refreshTracks() const;

    QListWidget* m_playlistListWidget;
    QListWidget* m_tracksListWidget;

    std::vector<Playlist> m_playlists;
    int m_currentPlaylistId = -1;
};
#endif // PHANTOMPLAYER_PLAYLISTCOMPONENT_H
