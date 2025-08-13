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

void PlaylistComponent::updateTracks(const std::vector<Track>&) {
    // This method needs to be reworked since Playlist doesn't have a tracks member
    // We store track indices, not the actual tracks
    refreshTracks();
}

void PlaylistComponent::refreshPlaylists() const {
    m_playlistListWidget->clear();
    for (int i = 0; i < m_playlists.size(); ++i) {
        const auto& playlist = m_playlists[i];
        auto* item = new QListWidgetItem(playlist.name);
        item->setData(Qt::UserRole, i); // Use index as ID
        m_playlistListWidget->addItem(item);
    }
}

void PlaylistComponent::refreshTracks() const {
    m_tracksListWidget->clear();
    if (m_currentPlaylistId >= 0 && m_currentPlaylistId < m_playlists.size()) {
        const auto& playlist = m_playlists[m_currentPlaylistId];
        // TODO: We need allTracks here to display them, but we don't have access to them
        // For now, just display indices
        for (const int trackIndex : playlist.trackIndices) {
            auto* item = new QListWidgetItem(QString("Track #%1").arg(trackIndex));
            item->setData(Qt::UserRole, trackIndex);
            m_tracksListWidget->addItem(item);
        }
    }
}

void PlaylistComponent::onPlaylistSelected(const QListWidgetItem* item) {
    if (!item) return;
    const int playlistId = item->data(Qt::UserRole).toInt();
    m_currentPlaylistId = playlistId;
    refreshTracks();
    emit playlistSelected(playlistId);
}

void PlaylistComponent::onTrackDoubleClicked(const QListWidgetItem* item) {
    if (!item) return;
    const int trackIndex = item->data(Qt::UserRole).toInt();
    // We need to emit the trackIndex, not the track itself
    // The signal should be modified to accept an index
    emit trackDoubleClicked(trackIndex);
}
