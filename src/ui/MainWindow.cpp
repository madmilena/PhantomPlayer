#include "MainWindow.h"
#include "ui/PlayerControlsWidget.h"
#include <QTabWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVariant>
#include <filesystem>
#include <QFont>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QDir>
#include <iostream>

#include "../core/AudioEngine.h"

namespace fs = std::filesystem;

// Função auxiliar para formatar a duração de segundos para MM:SS
QString formatDuration(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

MainWindow::MainWindow(PlaybackService* playbackService, PlaylistManager* playlistManager, QWidget *parent)
    : QMainWindow(parent), m_playbackService(playbackService), m_playlistManager(playlistManager) {
    
    setWindowTitle("Phantom Player");
    resize(1000, 600);
    setupUI();
    updateLibraryTab(m_playbackService->getTracks());
    m_playerControls->onVolumeChanged(m_playbackService->getInitialVolume());
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    createWidgets();
    setupLayouts();
    setupConnections();
}

void MainWindow::createWidgets() {
    m_tabWidget = new QTabWidget(this);
    m_searchBar = new QLineEdit(this);
    m_albumArtLabel = new QLabel("Nenhuma música tocando", this);
    m_titleLabel = new QLabel("Selecione uma música", this);
    m_artistLabel = new QLabel("", this);
    m_playerControls = new PlayerControlsWidget(this);
    m_trackContextMenu = new QMenu(this);
    m_addToPlaylistMenu = m_trackContextMenu->addMenu("Adicionar à playlist");
}

void MainWindow::setupLayouts() {
    // Barra de Menu
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("Arquivo");
    QAction* saveAction = new QAction("Salvar Playlists...", this);
    QAction* loadAction = new QAction("Carregar Playlists...", this);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSavePlaylists);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadPlaylists);
    
    // Layout principal
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto* mainLayout = new QHBoxLayout(centralWidget);

    auto* leftColumnWidget = new QWidget(this);
    auto* leftColumnLayout = new QVBoxLayout(leftColumnWidget);
    m_searchBar->setPlaceholderText("Buscar na biblioteca...");
    auto* newPlaylistButton = new QPushButton("+ Nova Playlist");
    
    auto* libraryListWidget = new QListWidget(this);
    libraryListWidget->setObjectName("Library");
    m_tabWidget->addTab(libraryListWidget, "Biblioteca");

    leftColumnLayout->addWidget(m_searchBar);
    leftColumnLayout->addWidget(m_tabWidget);
    leftColumnLayout->addWidget(newPlaylistButton);
    mainLayout->addWidget(leftColumnWidget, 2);

    auto* rightColumnWidget = new QWidget(this);
    auto* rightColumnLayout = new QVBoxLayout(rightColumnWidget);
    mainLayout->addWidget(rightColumnWidget, 1);
    
    m_albumArtLabel->setAlignment(Qt::AlignCenter);
    m_albumArtLabel->setMinimumSize(250, 250);
    rightColumnLayout->addWidget(m_albumArtLabel);

    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_artistLabel->setAlignment(Qt::AlignCenter);

    rightColumnLayout->addWidget(m_titleLabel);
    rightColumnLayout->addWidget(m_artistLabel);
    rightColumnLayout->addStretch();
    rightColumnLayout->addWidget(m_playerControls);

    connect(newPlaylistButton, &QPushButton::clicked, this, &MainWindow::createNewPlaylist);
}

void MainWindow::setupConnections() {
    connect(m_playerControls, &PlayerControlsWidget::playPauseClicked, m_playbackService, &PlaybackService::togglePlayPause);
    connect(m_playerControls, &PlayerControlsWidget::stopClicked, m_playbackService, &PlaybackService::stop);
    connect(m_playerControls, &PlayerControlsWidget::nextClicked, m_playbackService, &PlaybackService::next);
    connect(m_playerControls, &PlayerControlsWidget::prevClicked, m_playbackService, &PlaybackService::prev);
    connect(m_playerControls, &PlayerControlsWidget::shuffleToggled, m_playbackService, &PlaybackService::setShuffle);
    connect(m_playerControls, &PlayerControlsWidget::volumeChanged, this, [this](int value){ m_playbackService->setVolume(static_cast<float>(value)); });
    connect(m_playerControls, &PlayerControlsWidget::seeked, m_playbackService, &PlaybackService::seek);
    connect(m_playerControls, &PlayerControlsWidget::repeatClicked, this, &MainWindow::repeatButtonClicked);

    connect(m_playbackService, &PlaybackService::trackChanged, this, &MainWindow::onTrackChanged);
    connect(m_playbackService, &PlaybackService::playbackStateChanged, m_playerControls, &PlayerControlsWidget::onPlaybackStateChanged);
    connect(m_playbackService, &PlaybackService::progressUpdated, m_playerControls, &PlayerControlsWidget::onProgressUpdated);
    connect(m_playbackService, &PlaybackService::volumeChanged, m_playerControls, &PlayerControlsWidget::onVolumeChanged);
    connect(m_playlistManager, &PlaylistManager::playlistsChanged, this, &MainWindow::onPlaylistsChanged);

    connect(m_searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchQueryChanged);
    
    QListWidget* libraryList = qobject_cast<QListWidget*>(m_tabWidget->widget(0));
    if (libraryList) {
        connectListWidget(libraryList);
    }

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

void MainWindow::connectListWidget(QListWidget* listWidget) {
    if (!listWidget) return;
    
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::trackDoubleClicked);

    if (listWidget->objectName() == "Library") {
        listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(listWidget, &QWidget::customContextMenuRequested, this, &MainWindow::showTrackContextMenu);
    } else {
        listWidget->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

void MainWindow::updateLibraryTab(const std::vector<Track>& tracks) {
    QListWidget* libraryList = qobject_cast<QListWidget*>(m_tabWidget->widget(0));
    if (!libraryList) return;

    libraryList->clear();
    for (int i = 0; i < tracks.size(); ++i) {
        const auto& track = tracks[i];
        QString durationStr = formatDuration(track.durationInSeconds);
        QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
        if (track.artist.empty() || track.title.empty()) {
            displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
        }
        auto* item = new QListWidgetItem(displayText, libraryList);
        item->setData(Qt::UserRole, QVariant::fromValue(i));
    }
}

void MainWindow::onPlaylistsChanged() {
    const auto& playlists = m_playlistManager->getPlaylists();
    const auto& allTracks = m_playbackService->getTracks();

    while (m_tabWidget->count() > playlists.size() + 1) {
        m_tabWidget->removeTab(m_tabWidget->count() - 1);
    }

    for (int i = 0; i < playlists.size(); ++i) {
        QListWidget* playlistWidget;
        int tabIndex = i + 1;

        if (tabIndex < m_tabWidget->count()) {
            playlistWidget = qobject_cast<QListWidget*>(m_tabWidget->widget(tabIndex));
            m_tabWidget->setTabText(tabIndex, playlists[i].name);
        } else {
            playlistWidget = new QListWidget(this);
            m_tabWidget->addTab(playlistWidget, playlists[i].name);
            connectListWidget(playlistWidget);
        }
        
        playlistWidget->clear();
        for (int trackIndex : playlists[i].trackIndices) {
            if(trackIndex >= 0 && trackIndex < allTracks.size()){
                 const auto& track = allTracks[trackIndex];
                 QString durationStr = formatDuration(track.durationInSeconds);
                 QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
                 if (track.artist.empty() || track.title.empty()) {
                     displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
                 }
                 auto* item = new QListWidgetItem(displayText, playlistWidget);
                 item->setData(Qt::UserRole, QVariant::fromValue(trackIndex));
            }
        }
    }
}

void MainWindow::onTrackChanged(const Track& track, int index) {
    setWindowTitle("Tocando: " + QString::fromStdString(track.title));
    m_titleLabel->setText(QString::fromStdString(track.title));
    m_artistLabel->setText(QString::fromStdString(track.artist));
    
    QListWidget* libraryList = qobject_cast<QListWidget*>(m_tabWidget->widget(0));
    if(libraryList) {
        for(int i = 0; i < libraryList->count(); ++i) {
            if(libraryList->item(i)->data(Qt::UserRole).toInt() == index) {
                libraryList->setCurrentRow(i);
                break;
            }
        }
    }

    if (!track.albumArt.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(track.albumArt);
        m_albumArtLabel->setPixmap(pixmap.scaled(m_albumArtLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        m_albumArtLabel->clear();
        m_albumArtLabel->setText("Nenhuma capa de álbum");
    }
}

void MainWindow::trackDoubleClicked(QListWidgetItem* item) {
    int originalIndex = item->data(Qt::UserRole).toInt();
    m_playbackService->playTrack(originalIndex);
}

void MainWindow::repeatButtonClicked() {
    switch(m_repeatMode) {
        case RepeatMode::None:      m_repeatMode = RepeatMode::RepeatAll; break;
        case RepeatMode::RepeatAll: m_repeatMode = RepeatMode::RepeatOne; break;
        case RepeatMode::RepeatOne: m_repeatMode = RepeatMode::None;      break;
    }
    m_playbackService->setRepeatMode(m_repeatMode);
    m_playerControls->setRepeatButtonMode(m_repeatMode);
}

void MainWindow::onSearchQueryChanged(const QString &text) {
    auto* currentList = qobject_cast<QListWidget*>(m_tabWidget->currentWidget());
    if(!currentList) return;

    QString lowerCaseQuery = text.toLower();
    for (int i = 0; i < currentList->count(); ++i) {
        QListWidgetItem* item = currentList->item(i);
        QString itemText = item->text().toLower();
        bool matches = itemText.contains(lowerCaseQuery);
        item->setHidden(!matches);
    }
}

void MainWindow::showTrackContextMenu(const QPoint &pos) {
    QListWidget* libraryList = qobject_cast<QListWidget*>(m_tabWidget->widget(0));
    if (!libraryList || !libraryList->itemAt(pos)) return;

    m_addToPlaylistMenu->clear();
    const auto& playlists = m_playlistManager->getPlaylists();
    if (playlists.empty()) {
        m_addToPlaylistMenu->setEnabled(false);
    } else {
        m_addToPlaylistMenu->setEnabled(true);
        for (int i = 0; i < playlists.size(); ++i) {
            QAction* playlistAction = m_addToPlaylistMenu->addAction(playlists[i].name);
            playlistAction->setData(QVariant::fromValue(i));
            connect(playlistAction, &QAction::triggered, this, [this, playlistAction]{
                addTrackToSelectedPlaylist(playlistAction);
            });
        }
    }
    m_trackContextMenu->popup(libraryList->viewport()->mapToGlobal(pos));
}

void MainWindow::createNewPlaylist() {
    bool ok;
    QString name = QInputDialog::getText(this, "Nova Playlist", "Nome da Playlist:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        m_playlistManager->createNewPlaylist(name);
    }
}

void MainWindow::addTrackToSelectedPlaylist(QAction* action) {
    if (!action) return;
    int playlistIndexInManager = action->data().toInt();
    
    QListWidget* libraryList = qobject_cast<QListWidget*>(m_tabWidget->widget(0));
    auto selectedItems = libraryList->selectedItems();
    if (!selectedItems.empty()) {
        int trackIndex = selectedItems.first()->data(Qt::UserRole).toInt();
        m_playlistManager->addTrackToPlaylist(playlistIndexInManager, trackIndex);
    }
}

void MainWindow::onTabChanged(int index) {
    if (!m_searchBar->text().isEmpty()) {
        m_searchBar->clear();
    } else {
        onSearchQueryChanged("");
    }
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