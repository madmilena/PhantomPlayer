#include "PlaylistTabWidget.h"
#include "ui/components/PlaylistComponent.h"
#include <QDebug>
#include <algorithm>

PlaylistTabWidget::PlaylistTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, &PlaylistTabWidget::onTabCloseRequested);
}

PlaylistTabWidget::~PlaylistTabWidget() = default;

void PlaylistTabWidget::updatePlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks) {
    clear();

    for (size_t i = 0; i < playlists.size(); ++i) {
        const auto& playlist = playlists[i];

        auto* playlistComponent = new PlaylistComponent(this);

        std::vector<Track> playlistTracks;
        // --- CORREÇÃO AQUI: USANDO O GETTER ---
        for (int trackId : playlist.getTrackIds()) {
            if (auto it = std::find_if(allTracks.begin(), allTracks.end(), [trackId](const Track& t){ return t.trackId == trackId; }); it != allTracks.end()) {
                playlistTracks.push_back(*it);
            }
        }
        playlistComponent->updateTrackList(playlistTracks);

        // --- E CORREÇÃO AQUI: USANDO O GETTER ---
        addTab(playlistComponent, QString::fromStdString(playlist.getName()));

        connect(playlistComponent, &PlaylistComponent::trackDoubleClicked, this, &PlaylistTabWidget::trackDoubleClicked);

        connect(playlistComponent, &PlaylistComponent::removeTrackRequested, this, [this, i](int trackIndex) {
            qDebug() << "[PlaylistTabWidget] Repassando pedido de remoção. Playlist:" << i << ", Faixa:" << trackIndex;
            emit removeTrackFromPlaylist(i, trackIndex);
        });
    }
}

void PlaylistTabWidget::onTabCloseRequested(int index) {
    emit playlistClosed(index);
}