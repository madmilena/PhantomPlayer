#include "PlaylistComponent.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QMenu>
#include <QDebug>
#include <algorithm> // Necessário para std::find_if

PlaylistComponent::PlaylistComponent(int playlistId, QWidget *parent)
    : QWidget(parent), m_playlistId(playlistId)
{
    auto* layout = new QVBoxLayout(this);
    m_trackList = new QListWidget(this);
    layout->addWidget(m_trackList);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_trackList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_trackList, &QListWidget::customContextMenuRequested, this, &PlaylistComponent::onCustomContextMenuRequested);
    connect(m_trackList, &QListWidget::itemDoubleClicked, this, &PlaylistComponent::onItemDoubleClicked);
}

void PlaylistComponent::updateTracks(const std::vector<int>& trackIds, const std::vector<Track>& allTracks) {
    m_trackList->clear();
    for (const int currentTrackId : trackIds) {
        // --- CORREÇÃO FINAL AQUI ---
        // Usando o nome correto da variável: .trackId
        if (auto it = std::find_if(allTracks.begin(), allTracks.end(), [currentTrackId](const Track& t){ return t.trackId == currentTrackId; }); it != allTracks.end()) {
            const Track& track = *it;

            // Usando o nome correto da variável: .title
            auto* item = new QListWidgetItem(QString::fromStdString(track.title));

            // Usando o nome correto da variável: .trackId
            item->setData(Qt::UserRole, track.trackId);
            m_trackList->addItem(item);
        }
    }
}

void PlaylistComponent::onCustomContextMenuRequested(const QPoint &pos) {
    if (m_trackList->itemAt(pos) == nullptr) {
        return;
    }

    QMenu contextMenu(this);
    QAction* removeAction = contextMenu.addAction("Remover da Playlist");

    connect(removeAction, &QAction::triggered, this, &PlaylistComponent::onRemoveTrackTriggered);

    contextMenu.exec(m_trackList->mapToGlobal(pos));
}

void PlaylistComponent::onRemoveTrackTriggered() {
    QListWidgetItem* currentItem = m_trackList->currentItem();
    if (currentItem) {
        const int trackIndex = m_trackList->row(currentItem);
        qDebug() << "[PASSO 1 - PlaylistComponent] Ação 'Remover' foi clicada! Emitindo sinal para remover a faixa no índice:" << trackIndex;
        emit removeTrackRequested(trackIndex);
    }
}

void PlaylistComponent::onItemDoubleClicked(QListWidgetItem *item) {
    emit trackDoubleClicked(item);
}