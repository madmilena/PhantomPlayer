#ifndef SPOTIFYCLONE_MAINWINDOW_H
#define SPOTIFYCLONE_MAINWINDOW_H

#include <QMainWindow>
#include "services/PlaybackService.h"
#include "services/PlaylistManager.h"

// Forward declarations
class PlayerControlsWidget;
class TrackDetailsComponent;
class LibraryTabWidget;
class PlaylistTabWidget;
class QTabWidget;
class QListWidgetItem;

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(PlaybackService* playbackService, PlaylistManager* playlistManager, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onTrackChanged(const Track& track) const;
    void createNewPlaylist();
    void onPlaylistsChanged() const;
    void addTrackToPlaylist(int trackId) const;
    void deletePlaylist(int playlistIndex) const;
    void removeTrackFromPlaylist(int playlistIndex, int trackIndex) const;
    void onSavePlaylists();
    void onLoadPlaylists();
    void onTabChanged(int index);

    // --- CORREÇÃO AQUI: NOVO SLOT PARA INTERMEDIAR A CONEXÃO ---
    void onPlaylistTrackDoubleClicked(QListWidgetItem* item);

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