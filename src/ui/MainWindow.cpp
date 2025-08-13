#include "MainWindow.h"
#include "components/PlayerControlsWidget.h"
#include "components/TrackDetailsComponent.h"
#include "tabs/LibraryTabWidget.h"
#include "tabs/PlaylistTabWidget.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QTabWidget>
#include <iostream>

MainWindow::MainWindow(PlaybackService* playbackService, PlaylistManager* playlistManager, QWidget *parent)
    : QMainWindow(parent), m_playbackService(playbackService), m_playlistManager(playlistManager) {
    
    setWindowTitle("Phantom Player");
    resize(1000, 600);
    setupUI();
    setupConnections();
    
    m_libraryTab->updateTrackList(m_playbackService->getTracks());
    m_playerControls->onVolumeChanged(m_playbackService->getInitialVolume());
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    createWidgets();
    setupLayouts();
}

void MainWindow::createWidgets() {
    m_libraryTab = new LibraryTabWidget(this);
    m_playlistTabs = new PlaylistTabWidget(this);
    m_trackDetails = new TrackDetailsComponent(this);
    m_playerControls = new PlayerControlsWidget(this);
}

void MainWindow::setupLayouts() {
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("Arquivo");
    QAction* saveAction = new QAction("Salvar Playlists...", this);
    QAction* loadAction = new QAction("Carregar Playlists...", this);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSavePlaylists);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadPlaylists);

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto* mainLayout = new QHBoxLayout(centralWidget);

    auto* leftColumnWidget = new QWidget(this);
    auto* leftColumnLayout = new QVBoxLayout(leftColumnWidget);
    auto* newPlaylistButton = new QPushButton("+ Nova Playlist");
    connect(newPlaylistButton, &QPushButton::clicked, this, &MainWindow::createNewPlaylist);
    
    auto* mainTabs = new QTabWidget(this);
    mainTabs->addTab(m_libraryTab, "Biblioteca");
    mainTabs->addTab(m_playlistTabs, "Playlists");
    
    leftColumnLayout->addWidget(mainTabs);
    leftColumnLayout->addWidget(newPlaylistButton);
    mainLayout->addWidget(leftColumnWidget, 2);

    auto* rightColumnWidget = new QWidget(this);
    auto* rightColumnLayout = new QVBoxLayout(rightColumnWidget);
    rightColumnLayout->addWidget(m_trackDetails);
    rightColumnLayout->addStretch();
    rightColumnLayout->addWidget(m_playerControls);
    mainLayout->addWidget(rightColumnWidget, 1);
}

void MainWindow::setupConnections() {
    connect(m_playerControls, &PlayerControlsWidget::playPauseClicked, m_playbackService, &PlaybackService::togglePlayPause);
    connect(m_playerControls, &PlayerControlsWidget::stopClicked, m_playbackService, &PlaybackService::stop);
    connect(m_playerControls, &PlayerControlsWidget::nextClicked, m_playbackService, &PlaybackService::next);
    connect(m_playerControls, &PlayerControlsWidget::prevClicked, m_playbackService, &PlaybackService::prev);
    connect(m_playerControls, &PlayerControlsWidget::shuffleToggled, m_playbackService, &PlaybackService::setShuffle);
    connect(m_playerControls, &PlayerControlsWidget::volumeChanged, this, [this](int value){ m_playbackService->setVolume(static_cast<float>(value)); });
    connect(m_playerControls, &PlayerControlsWidget::seeked, m_playbackService, &PlaybackService::seek);

    connect(m_playbackService, &PlaybackService::trackChanged, this, &MainWindow::onTrackChanged);
    connect(m_playbackService, &PlaybackService::playbackStateChanged, m_playerControls, &PlayerControlsWidget::onPlaybackStateChanged);
    connect(m_playbackService, &PlaybackService::progressUpdated, m_playerControls, &PlayerControlsWidget::onProgressUpdated);
    connect(m_playbackService, &PlaybackService::volumeChanged, m_playerControls, &PlayerControlsWidget::onVolumeChanged);
    
    connect(m_playlistManager, &PlaylistManager::playlistsChanged, this, &MainWindow::onPlaylistsChanged);
    
    connect(m_libraryTab, &LibraryTabWidget::trackDoubleClicked, m_playbackService, &PlaybackService::playTrack);
    connect(m_playlistTabs, &PlaylistTabWidget::trackDoubleClicked, m_playbackService, &PlaybackService::playTrack);
    connect(m_libraryTab, &LibraryTabWidget::addToPlaylistRequested, this, &MainWindow::addTrackToPlaylist);
}

void MainWindow::onTrackChanged(const Track& track) {
    m_trackDetails->updateDetails(track);
}

void MainWindow::createNewPlaylist() {
    bool ok;
    QString name = QInputDialog::getText(this, "Nova Playlist", "Nome da Playlist:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        m_playlistManager->createNewPlaylist(name);
    }
}

void MainWindow::addTrackToPlaylist(int trackIndex) {
    int playlistIndex = m_playlistTabs->currentIndex();
    if (playlistIndex >= 0) {
        m_playlistManager->addTrackToPlaylist(playlistIndex, trackIndex);
    } else {
        createNewPlaylist();
    }
}

void MainWindow::onPlaylistsChanged() {
    m_playlistTabs->updatePlaylists(m_playlistManager->getPlaylists(), m_playbackService->getTracks());
}

void MainWindow::onSavePlaylists() {
    QString filePath = QFileDialog::getSaveFileName(this, "Salvar Playlists", QDir::homePath(), "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        m_playlistManager->savePlaylistsToFile(filePath);
    }
}

void MainWindow::onLoadPlaylists() {
    QString filePath = QFileDialog::getOpenFileName(this, "Carregar Playlists", QDir::homePath(), "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        m_playlistManager->loadPlaylistsFromFile(filePath);
    }
}