#ifndef PHANTOMPLAYER_PLAYLISTTABWIDGET_H
#define PHANTOMPLAYER_PLAYLISTTABWIDGET_H

#include <QTabWidget>
#include <vector>
#include "core/Playlist.h"
#include "core/Track.h"

class QListWidgetItem;

class PlaylistTabWidget : public QTabWidget {
    Q_OBJECT

public:
    explicit PlaylistTabWidget(QWidget* parent = nullptr);
    ~PlaylistTabWidget() override;

    void updatePlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks);

    signals:
        // Sinais que a MainWindow vai ouvir.
        void trackDoubleClicked(QListWidgetItem* item);
    void removeTrackFromPlaylist(int playlistIndex, int trackIndex);
    void playlistClosed(int playlistIndex);

private slots:
    // Slot para lidar com o fechamento de abas.
    void onTabCloseRequested(int index);
};

#endif //PHANTOMPLAYER_PLAYLISTTABWIDGET_H