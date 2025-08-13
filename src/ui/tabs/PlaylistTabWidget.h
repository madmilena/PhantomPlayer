#ifndef PHANTOMPLAYER_PLAYLISTTABWIDGET_H
#define PHANTOMPLAYER_PLAYLISTTABWIDGET_H

#include <QTabWidget>
#include <vector>
#include "core/Playlist.h"
#include "core/Track.h" // <-- ADICIONADO

class QListWidget;
class QListWidgetItem;

class PlaylistTabWidget : public QTabWidget { // <-- MUDOU DE BaseTab PARA QTabWidget
    Q_OBJECT

public:
    explicit PlaylistTabWidget(QWidget* parent = nullptr);
    void updatePlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks);

signals:
    void trackDoubleClicked(int trackIndex);

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);

private:
    QString formatDuration(int totalSeconds);
};

#endif //PHANTOMPLAYER_PLAYLISTTABWIDGET_H