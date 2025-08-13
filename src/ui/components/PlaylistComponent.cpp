#include "PlaylistComponent.h"
#include <QVBoxLayout>
#include <QListWidgetItem>

PlaylistComponent::PlaylistComponent(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

    m_playlistListWidget = new QListWidget(this);
    m_tracksListWidget = new QListWidget(this);

    layout->addWidget(m_playlistListWidget);
    layout->addWidget(m_tracksListWidget);

    connect(m_playlistListWidget, &QListWidget::itemClicked, this, &PlaylistComponent::onPlaylistSelected);
    connect(m_tracksListWidget, &QListWidget::itemDoubleClicked, this, &PlaylistComponent::onTrackDoubleClicked);
}

void PlaylistComponent::setPlaylists(const std::vector<Playlist>& playlists) {
    m_playlists = playlists;
    refreshPlaylists();
}

void PlaylistComponent::setCurrentPlaylist(int playlistId) {
    m_currentPlaylistId = playlistId;
    refreshTracks();
}

void PlaylistComponent::updateTracks(const std::vector<Track>& tracks) {
    for (auto& playlist : m_playlists) {
        if (playlist.id == m_currentPlaylistId) {
            playlist.tracks = tracks;
            break;
        }
    }
    refreshTracks();
}

void PlaylistComponent::refreshPlaylists() {
    m_playlistListWidget->clear();
    for (const auto& playlist : m_playlists) {
        auto* item = new QListWidgetItem(QString::fromStdString(playlist.name));
        item->setData(Qt::UserRole, playlist.id);
        m_playlistListWidget->addItem(item);
    }
}

void PlaylistComponent::refreshTracks() {
    m_tracksListWidget->clear();
    for (const auto& playlist : m_playlists) {
        if (playlist.id == m_currentPlaylistId) {
            for (const auto& track : playlist.tracks) {
                auto* item = new QListWidgetItem(QString("%1 - %2").arg(track.artist, track.title));
                item->setData(Qt::UserRole, track.id);
                m_tracksListWidget->addItem(item);
            }
            break;
        }
    }
}

void PlaylistComponent::onPlaylistSelected(QListWidgetItem* item) {
    if (!item) return;
    int playlistId = item->data(Qt::UserRole).toInt();
    m_currentPlaylistId = playlistId;
    refreshTracks();
    emit playlistSelected(playlistId);
}

void PlaylistComponent::onTrackDoubleClicked(QListWidgetItem* item) {
    if (!item) return;
    int trackId = item->data(Qt::UserRole).toInt();
    for (const auto& playlist : m_playlists) {
        if (playlist.id == m_currentPlaylistId) {
            for (const auto& track : playlist.tracks) {
                if (track.id == trackId) {
                    emit trackDoubleClicked(track);
                    return;
                }
            }
        }
    }
}
