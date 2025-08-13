#include "MainWindow.h"
#include "components/PlayerControlsWidget.h"
#include "components/TrackDetailsComponent.h"
#include "tabs/LibraryTabWidget.h"
#include "tabs/PlaylistTabWidget.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <iostream>

MainWindow::MainWindow(PlaybackService* playbackService, PlaylistManager* playlistManager, QWidget *parent)
    : QMainWindow(parent), m_playbackService(playbackService), m_playlistManager(playlistManager),
      m_libraryTab(nullptr), m_playerControls(nullptr)
{
    setWindowTitle("Phantom Player");
    resize(1000, 600);
    setupUI();
    setupConnections();

    m_libraryTab->updateTrackList(m_playbackService->getTracks());
    m_playerControls->onVolumeChanged(m_playbackService->getInitialVolume());
    onPlaylistsChanged();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    createWidgets();
    setupLayouts();
}

void MainWindow::createWidgets() {
    m_mainTabs = new QTabWidget(this);
    m_libraryTab = new LibraryTabWidget(this);
    m_playlistTabs = new PlaylistTabWidget(this);
    m_trackDetails = new TrackDetailsComponent(this);
    m_playerControls = new PlayerControlsWidget(this);
}

void MainWindow::setupLayouts() {
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("Arquivo");
    const auto saveAction = new QAction("Salvar Playlists...", this);
    const auto loadAction = new QAction("Carregar Playlists...", this);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto* mainLayout = new QHBoxLayout(centralWidget);

    auto* leftColumnWidget = new QWidget(this);
    auto* leftColumnLayout = new QVBoxLayout(leftColumnWidget);
    auto* newPlaylistButton = new QPushButton("+ Nova Playlist");
    
    m_mainTabs->addTab(m_libraryTab, "Biblioteca");
    m_mainTabs->addTab(m_playlistTabs, "Playlists");
    
    leftColumnLayout->addWidget(m_mainTabs);
    leftColumnLayout->addWidget(newPlaylistButton);
    mainLayout->addWidget(leftColumnWidget, 2);

    auto* rightColumnWidget = new QWidget(this);
    auto* rightColumnLayout = new QVBoxLayout(rightColumnWidget);
    rightColumnLayout->addWidget(m_trackDetails);
    rightColumnLayout->addStretch();
    rightColumnLayout->addWidget(m_playerControls);
    mainLayout->addWidget(rightColumnWidget, 1);

    connect(newPlaylistButton, &QPushButton::clicked, this, &MainWindow::createNewPlaylist);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSavePlaylists);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadPlaylists);
}

void MainWindow::setupConnections() {
    connect(m_playerControls, &PlayerControlsWidget::playPauseClicked, m_playbackService, &PlaybackService::togglePlayPause);
    connect(m_playerControls, &PlayerControlsWidget::stopClicked, m_playbackService, &PlaybackService::stop);
    connect(m_playerControls, &PlayerControlsWidget::nextClicked, m_playbackService, &PlaybackService::next);
    connect(m_playerControls, &PlayerControlsWidget::prevClicked, m_playbackService, &PlaybackService::prev);
    connect(m_playerControls, &PlayerControlsWidget::shuffleToggled, m_playbackService, &PlaybackService::setShuffle);
    connect(m_playerControls, &PlayerControlsWidget::volumeChanged, this, [this](int value){ m_playbackService->setVolume(static_cast<float>(value)); });
    connect(m_playerControls, &PlayerControlsWidget::seeked, m_playbackService, &PlaybackService::seek);
    connect(m_playerControls, &PlayerControlsWidget::repeatClicked, this, [this](){
        RepeatMode currentMode = m_playbackService->getRepeatMode();
        const auto nextMode = static_cast<RepeatMode>((static_cast<int>(currentMode) + 1) % 3);
        m_playbackService->setRepeatMode(nextMode);
        m_playerControls->setRepeatButtonMode(nextMode);
    });
    connect(m_playbackService, &PlaybackService::trackChanged, this, &MainWindow::onTrackChanged);
    connect(m_playbackService, &PlaybackService::playbackStateChanged, m_playerControls, &PlayerControlsWidget::onPlaybackStateChanged);
    connect(m_playbackService, &PlaybackService::progressUpdated, m_playerControls, &PlayerControlsWidget::onProgressUpdated);
    connect(m_playbackService, &PlaybackService::volumeChanged, m_playerControls, &PlayerControlsWidget::onVolumeChanged);
    connect(m_playlistManager, &PlaylistManager::playlistsChanged, this, &MainWindow::onPlaylistsChanged);
    connect(m_libraryTab, &LibraryTabWidget::trackDoubleClicked, m_playbackService, &PlaybackService::playTrack);
    connect(m_playlistTabs, &PlaylistTabWidget::trackDoubleClicked, m_playbackService, &PlaybackService::playTrack);
    connect(m_libraryTab, &LibraryTabWidget::addToPlaylistRequested, this, &MainWindow::addTrackToPlaylist);
    connect(m_playlistTabs, &PlaylistTabWidget::playlistClosed, this, &MainWindow::deletePlaylist);
    connect(m_mainTabs, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

void MainWindow::onTrackChanged(const Track& track) const
{
    m_trackDetails->updateDetails(track);
}

void MainWindow::createNewPlaylist() {
    bool ok;
    if (const QString name = QInputDialog::getText(this, "Nova Playlist", "Nome da Playlist:", QLineEdit::Normal, "", &ok); ok && !name.isEmpty()) {
        m_playlistManager->createNewPlaylist(name);
    }
}

void MainWindow::addTrackToPlaylist(const int trackIndex) const
{
    if (const int playlistIndex = m_playlistTabs->currentIndex(); playlistIndex >= 0) {
        m_playlistManager->addTrackToPlaylist(playlistIndex, trackIndex);
    } else {
        std::cout << "Nenhuma playlist selecionada para adicionar a musica." << std::endl;
    }
}

void MainWindow::deletePlaylist(const int index) const
{
    m_playlistManager->deletePlaylist(index);
}

void MainWindow::onPlaylistsChanged() const
{
    m_playlistTabs->updatePlaylists(m_playlistManager->getPlaylists(), m_playbackService->getTracks());
}

void MainWindow::onSavePlaylists() {
    if (const QString filePath = QFileDialog::getSaveFileName(this, "Salvar Playlists", QDir::homePath(), "JSON Files (*.json)"); !filePath.isEmpty()) {
        m_playlistManager->savePlaylistsToFile(filePath);
    }
}

void MainWindow::onLoadPlaylists() {
    if (const QString filePath = QFileDialog::getOpenFileName(this, "Carregar Playlists", QDir::homePath(), "JSON Files (*.json)"); !filePath.isEmpty()) {
        m_playlistManager->loadPlaylistsFromFile(filePath);
    }
}

void MainWindow::onTabChanged(const int index) {
    if (index == 1) {
        m_libraryTab->clearSearch();
    }
}

// ****** FUNÇÃO QUE FALTAVA FOI ADICIONADA AQUI ******
void MainWindow::removeTrackFromPlaylist(int playlistId, int trackIndex) {
    // TODO: Adicionar a lógica para remover a faixa da playlist aqui.
    // Por exemplo:
    // PlaybackService::getInstance().removeTrackFromPlaylist(playlistId, trackIndex);
    qDebug() << "Removendo faixa" << trackIndex << "da playlist" << playlistId;
}

// Slot ajustado para receber o tipo correto do sinal
void MainWindow::playTrackFromPlaylist(QListWidgetItem *item) {
    // TODO: Adicionar a lógica para tocar a faixa aqui.
    // Você precisará extrair as informações da faixa do QListWidgetItem.
    qDebug() << "Tocando a faixa:" << item->text();
}