#ifndef PHANTOMPLAYER_PLAYLISTTABWIDGET_H
#define PHANTOMPLAYER_PLAYLISTTABWIDGET_H

#include <QTabWidget>
#include <vector>
#include "core/Playlist.h"
#include "core/Track.h"

class BaseTab;
class QListWidgetItem;

class PlaylistTabWidget : public QTabWidget {
    Q_OBJECT

public:
    explicit PlaylistTabWidget(QWidget* parent = nullptr);
    void updatePlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks);

    signals:
        void trackDoubleClicked(int trackIndex);
    void playlistClosed(int index);
    void removeTrackFromPlaylist(int playlistIndex, int trackIndex); // <-- SINAL ADICIONADO

private slots:
    void onTrackDoubleClicked(QListWidgetItem* item);
};

#endif //PHANTOMPLAYER_PLAYLISTTABWIDGET_H