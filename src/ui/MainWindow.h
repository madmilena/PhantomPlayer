#ifndef SPOTIFYCLONE_MAINWINDOW_H
#define SPOTIFYCLONE_MAINWINDOW_H

#include <QMainWindow>
#include "../services/PlaybackService.h"
#include "../services/PlaylistManager.h"

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
    // Slots que reagem a eventos dos serviços
    void onTrackChanged(const Track& track) const;
    void onPlaylistsChanged() const;

    // Slots que reagem a eventos da UI
    void createNewPlaylist();
    void addTrackToPlaylist(int trackIndex) const;
    void deletePlaylist(int index) const;
    void removeTrackFromPlaylist(int playlistId, int trackIndex);
    void playTrackFromPlaylist(QListWidgetItem *item);
    void onSavePlaylists();
    void onLoadPlaylists();
    void onTabChanged(int index);

private:
    // --- Funções de Configuração Refatoradas ---
    void setupUI();
    void createWidgets();
    void setupLayouts();

    // Função principal de conexões, agora mais limpa
    void setupConnections();
    // Funções auxiliares para organizar as conexões
    void setupPlaybackConnections();
    void setupPlaylistConnections();
    void setupGeneralUIConnections();


    // Ponteiros para serviços e componentes da UI
    PlaybackService* m_playbackService;
    PlaylistManager* m_playlistManager;

    QTabWidget* m_mainTabs;
    LibraryTabWidget* m_libraryTab;
    PlaylistTabWidget* m_playlistTabs;
    TrackDetailsComponent* m_trackDetails;
    PlayerControlsWidget* m_playerControls;
};

#endif //SPOTIFYCLONE_MAINWINDOW_H