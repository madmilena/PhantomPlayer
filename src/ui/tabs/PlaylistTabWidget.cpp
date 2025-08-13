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
    clear(); // Limpa todas as abas antigas

    for (size_t i = 0; i < playlists.size(); ++i) {
        const auto& playlist = playlists[i];

        // Cria um novo componente de playlist para a aba
        auto* playlistComponent = new PlaylistComponent(this);

        // Prepara a lista de faixas somente para esta playlist
        std::vector<Track> playlistTracks;
        for (int trackId : playlist.getTrackIds()) {
            if (auto it = std::find_if(allTracks.begin(), allTracks.end(), [trackId](const Track& t){ return t.trackId == trackId; }); it != allTracks.end()) {
                playlistTracks.push_back(*it);
            }
        }
        playlistComponent->updateTrackList(playlistTracks);

        // Adiciona a nova aba com o componente dentro
        addTab(playlistComponent, QString::fromStdString(playlist.getName()));

        // Conecta os sinais do componente aos sinais deste TabWidget
        connect(playlistComponent, &PlaylistComponent::trackDoubleClicked, this, &PlaylistTabWidget::trackDoubleClicked);

        // Conecta o sinal de remoção, adicionando o índice da playlist (i)
        connect(playlistComponent, &PlaylistComponent::removeTrackRequested, this, [this, i](int trackIndex) {
            qDebug() << "[PlaylistTabWidget] Repassando pedido de remoção. Playlist:" << i << ", Faixa:" << trackIndex;
            emit removeTrackFromPlaylist(i, trackIndex);
        });
    }
}

void PlaylistTabWidget::onTabCloseRequested(int index) {
    // Simplesmente emite um sinal para a MainWindow decidir o que fazer.
    emit playlistClosed(index);
}