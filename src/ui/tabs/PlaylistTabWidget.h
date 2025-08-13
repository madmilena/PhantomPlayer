#ifndef PHANTOMPLAYER_PLAYLISTTABWIDGET_H
#define PHANTOMPLAYER_PLAYLISTTABWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QMenu>

class PlaylistTabWidget : public QTabWidget {
    Q_OBJECT
public:
    explicit PlaylistTabWidget(QWidget* parent = nullptr);

    struct Playlist {
        QString name;
        std::vector<int> trackIndices;
    };

    void setPlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks);

signals:
    void trackSelected(int index);

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);
    void showContextMenu(const QPoint& pos);

private:
    void updatePlaylistTab(int tabIndex);

    std::vector<Playlist> m_playlists;
    std::vector<Track> m_allTracks;
};

#endif // PHANTOMPLAYER_PLAYLISTTABWIDGET_H
