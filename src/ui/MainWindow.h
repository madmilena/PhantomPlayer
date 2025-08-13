#ifndef SPOTIFYCLONE_MAINWINDOW_H
#define SPOTIFYCLONE_MAINWINDOW_H

#include <QMainWindow>
#include "../services/PlaybackService.h"
#include "../services/PlaylistManager.h"

class PlayerControlsWidget;
class TrackDetailsComponent;
class LibraryTabWidget;
class PlaylistTabWidget;
class QTabWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(PlaybackService* playbackService, PlaylistManager* playlistManager, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onTrackChanged(const Track& track);
    void createNewPlaylist();
    void addTrackToPlaylist(int trackIndex);
    void onPlaylistsChanged();
    void deletePlaylist(int index);
    void removeTrackFromPlaylist(int playlistIndex, int trackIndex); // <-- NOVO SLOT
    void onSavePlaylists();
    void onLoadPlaylists();

private:
    void setupUI();
    void createWidgets();
    void setupLayouts();
    void setupConnections();

    PlaybackService* m_playbackService;
    PlaylistManager* m_playlistManager;

    QTabWidget* m_mainTabs;
    LibraryTabWidget* m_libraryTab;
    PlaylistTabWidget* m_playlistTabs;
    TrackDetailsComponent* m_trackDetails;
    PlayerControlsWidget* m_playerControls;
};

#endif //SPOTIFYCLONE_MAINWINDOW_H