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
    m_mainTabs = new QTabWidget(this);
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
}

void MainWindow::setupConnections() {
    connect(m_playerControls, &PlayerControlsWidget::playPauseClicked, m_playbackService, &PlaybackService::togglePlayPause);
    connect(m_playerControls, &PlayerControlsWidget::stopClicked, m_playbackService, &PlaybackService::stop);
    connect(m_playerControls, &PlayerControlsWidget::nextClicked, m_playbackService, &PlaybackService::next);
    connect(m_playerControls, &PlayerControlsWidget::prevClicked, m_playbackService, &PlaybackService::prev);
    connect(m_playerControls, &PlayerControlsWidget::shuffleToggled, m_playbackService, &PlaybackService::setShuffle);
    connect(m_playerControls, &PlayerControlsWidget::volumeChanged, m_playbackService, [this](int value){ m_playbackService->setVolume(static_cast<float>(value)); });
    connect(m_playerControls, &PlayerControlsWidget::seeked, m_playbackService, &PlaybackService::seek);
    connect(m_playerControls, &PlayerControlsWidget::repeatClicked, this, &MainWindow::repeatButtonClicked);

    connect(m_playbackService, &PlaybackService::trackChanged, this, &MainWindow::onTrackChanged);
    connect(m_playbackService, &PlaybackService::playbackStateChanged, m_playerControls, &PlayerControlsWidget::onPlaybackStateChanged);
    connect(m_playbackService, &PlaybackService::progressUpdated, m_playerControls, &PlayerControlsWidget::onProgressUpdated);
    connect(m_playbackService, &PlaybackService::volumeChanged, m_playerControls, &PlayerControlsWidget::onVolumeChanged);
    connect(m_playlistManager, &PlaylistManager::playlistsChanged, this, &MainWindow::onPlaylistsChanged);

    connect(m_libraryTab, &LibraryTabWidget::trackDoubleClicked, m_playbackService, &PlaybackService::playTrack);
    connect(m_playlistTabs, &PlaylistTabWidget::trackDoubleClicked, m_playbackService, &PlaybackService::playTrack);
    connect(m_libraryTab, &LibraryTabWidget::addToPlaylistRequested, this, &MainWindow::addTrackToPlaylist);
    
    connect(m_mainTabs, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

void MainWindow::onTrackChanged(const Track& track) {
    m_trackDetails->updateTrackDetails(track);
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
        std::cout << "Nenhuma playlist selecionada para adicionar a musica." << std::endl;
    }
}

void MainWindow::onPlaylistsChanged() {
    m_playlistTabs->updatePlaylists(m_playlistManager->getPlaylists(), m_playbackService->getTracks());
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

// --- SLOTS PARA SALVAR E CARREGAR ---
void MainWindow::onSavePlaylists() {
    QString filePath = QFileDialog::getSaveFileName(this, "Salvar Playlists", QDir::homePath(), "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        if (m_playlistManager->savePlaylistsToFile(filePath)) {
            std::cout << "Playlists salvas com sucesso em: " << filePath.toStdString() << std::endl;
        } else {
            std::cerr << "Erro ao salvar playlists." << std::endl;
        }
    }
}

void MainWindow::onLoadPlaylists() {
    QString filePath = QFileDialog::getOpenFileName(this, "Carregar Playlists", QDir::homePath(), "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        if (m_playlistManager->loadPlaylistsFromFile(filePath)) {
            std::cout << "Playlists carregadas com sucesso de: " << filePath.toStdString() << std::endl;
        } else {
            std::cerr << "Erro ao carregar playlists." << std::endl;
        }
    }
}