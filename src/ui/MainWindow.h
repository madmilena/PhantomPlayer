#ifndef SPOTIFYCLONE_MAINWINDOW_H
#define SPOTIFYCLONE_MAINWINDOW_H

#include <QMainWindow>
#include "../services/PlaybackService.h"
#include "../services/PlaylistManager.h"
#include "../core/Track.h"

class QListWidget;
class QListWidgetItem;
class QLabel;
class PlayerControlsWidget;
class QLineEdit;
class QTabWidget;
class QMenu;
class QAction;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(PlaybackService* playbackService, PlaylistManager* playlistManager, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onTrackChanged(const Track& track, int index);
    void trackDoubleClicked(QListWidgetItem* item);
    void repeatButtonClicked();
    void onSearchQueryChanged(const QString& text);
    void showTrackContextMenu(const QPoint& pos);
    void createNewPlaylist();
    void addTrackToSelectedPlaylist(QAction* action);
    void onPlaylistsChanged();
    void onTabChanged(int index);
    
    // SLOTS QUE FALTAVAM
    void onSavePlaylists();
    void onLoadPlaylists();

private:
    void setupUI();
    void createWidgets();
    void setupLayouts();
    void setupConnections();
    void connectListWidget(QListWidget* listWidget);
    void updateLibraryTab(const std::vector<Track>& tracks);
    QString formatDuration(int totalSeconds);

    PlaybackService* m_playbackService;
    PlaylistManager* m_playlistManager;
    RepeatMode m_repeatMode = RepeatMode::None;

    QTabWidget* m_tabWidget;
    QLineEdit* m_searchBar;
    QLabel* m_albumArtLabel;
    QLabel* m_titleLabel;
    QLabel* m_artistLabel;
    PlayerControlsWidget* m_playerControls;
    QMenu* m_trackContextMenu;
    QMenu* m_addToPlaylistMenu;
};

#endif //SPOTIFYCLONE_MAINWINDOW_H