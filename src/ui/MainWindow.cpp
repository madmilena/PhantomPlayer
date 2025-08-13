#include "MainWindow.h"
#include "ui/PlayerControlsWidget.h"

#include <QTabWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QInputDialog>
#include <QVariant>
#include <filesystem>

namespace fs = std::filesystem;

MainWindow::MainWindow(PlaybackService* playbackService, PlaylistManager* playlistManager, QWidget *parent)
    : QMainWindow(parent), m_playbackService(playbackService), m_playlistManager(playlistManager) {
    
    setWindowTitle("Phantom Player");
    resize(1000, 600);

    setupUI();
    setupConnections();

    updateLibraryTab(m_playbackService->getTracks());
    m_playerControls->onVolumeChanged(m_playbackService->getInitialVolume());
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    m_tabWidget = new QTabWidget(this);
    m_searchBar = new QLineEdit(this);
    m_albumArtLabel = new QLabel("Nenhuma música tocando", this);
    m_titleLabel = new QLabel("Selecione uma música", this);
    m_artistLabel = new QLabel("", this);
    m_playerControls = new PlayerControlsWidget(this);
    m_trackContextMenu = new QMenu(this);
    m_addToPlaylistMenu = m_trackContextMenu->addMenu("Adicionar à playlist");

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto* mainLayout = new QHBoxLayout(centralWidget);

    auto* leftColumn = new QWidget(this);
    auto* leftLayout = new QVBoxLayout(leftColumn);

    auto* newPlaylistButton = new QPushButton("+ Nova Playlist", this);
    QListWidget* libraryListWidget = new QListWidget(this);
    libraryListWidget->setObjectName("Library");
    m_tabWidget->addTab(libraryListWidget, "Biblioteca");

    m_searchBar->setPlaceholderText("Buscar na biblioteca...");
    leftLayout->addWidget(m_searchBar);
    leftLayout->addWidget(m_tabWidget);
    leftLayout->addWidget(newPlaylistButton);

    mainLayout->addWidget(leftColumn, 2);

    auto* rightColumn = new QWidget(this);
    auto* rightLayout = new QVBoxLayout(rightColumn);

    m_albumArtLabel->setAlignment(Qt::AlignCenter);
    m_albumArtLabel->setMinimumSize(250, 250);
    rightLayout->addWidget(m_albumArtLabel);

    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_artistLabel->setAlignment(Qt::AlignCenter);

    rightLayout->addWidget(m_titleLabel);
    rightLayout->addWidget(m_artistLabel);
    rightLayout->addStretch();
    rightLayout->addWidget(m_playerControls);

    mainLayout->addWidget(rightColumn, 1);

    connect(newPlaylistButton, &QPushButton::clicked, this, &MainWindow::createNewPlaylist);

    connect(libraryListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::trackDoubleClicked);
    libraryListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(libraryListWidget, &QWidget::customContextMenuRequested, this, &MainWindow::showTrackContextMenu);
}

void MainWindow::setupConnections() {
    connect(m_playerControls, &PlayerControlsWidget::playPauseClicked, m_playbackService, &PlaybackService::togglePlayPause);
    connect(m_playerControls, &PlayerControlsWidget::stopClicked, m_playbackService, &PlaybackService::stop);
    connect(m_playerControls, &PlayerControlsWidget::nextClicked, m_playbackService, &PlaybackService::next);
    connect(m_playerControls, &PlayerControlsWidget::prevClicked, m_playbackService, &PlaybackService::prev);
    connect(m_playerControls, &PlayerControlsWidget::shuffleToggled, m_playbackService, &PlaybackService::setShuffle);
    connect(m_playerControls, &PlayerControlsWidget::volumeChanged, this, [this](int value){ m_playbackService->setVolume(static_cast<float>(value) / 100.f); });
    connect(m_playerControls, &PlayerControlsWidget::seeked, m_playbackService, &PlaybackService::seek);
    connect(m_playerControls, &PlayerControlsWidget::repeatClicked, this, &MainWindow::repeatButtonClicked);

    connect(m_playbackService, &PlaybackService::trackChanged, this, &MainWindow::onTrackChanged);
    connect(m_playbackService, &PlaybackService::playbackStateChanged, m_playerControls, &PlayerControlsWidget::onPlaybackStateChanged);
    connect(m_playbackService, &PlaybackService::progressUpdated, m_playerControls, &PlayerControlsWidget::onProgressUpdated);
    connect(m_playbackService, &PlaybackService::volumeChanged, m_playerControls, &PlayerControlsWidget::onVolumeChanged);

    connect(m_playlistManager, &PlaylistManager::playlistsChanged, this, &MainWindow::onPlaylistsChanged);
    connect(m_searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchQueryChanged);
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

QString MainWindow::formatDuration(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
