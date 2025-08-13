#include "PlaylistComponent.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QMenu>
#include <QDebug>

PlaylistComponent::PlaylistComponent(QWidget *parent)
    : QWidget(parent)
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

PlaylistComponent::~PlaylistComponent() = default;

void PlaylistComponent::updateTrackList(const std::vector<Track>& tracks) {
    m_trackList->clear();
    for (const auto& track : tracks) {
        // Usamos os nomes corretos das variáveis da sua struct Track
        auto* item = new QListWidgetItem(QString::fromStdString(track.artist + " - " + track.title));
        item->setData(Qt::UserRole, track.trackId);
        m_trackList->addItem(item);
    }
}

void PlaylistComponent::onItemDoubleClicked(QListWidgetItem *item) {
    // Simplesmente repassa o sinal para cima.
    emit trackDoubleClicked(item);
}

void PlaylistComponent::onCustomContextMenuRequested(const QPoint &pos) {
    QListWidgetItem* item = m_trackList->itemAt(pos);
    if (!item) {
        return;
    }

    QMenu contextMenu(this);
    QAction* removeAction = contextMenu.addAction("Remover da Playlist");

    // Conecta o clique na ação a uma lambda que emite o sinal.
    connect(removeAction, &QAction::triggered, this, [this, item]() {
        // Pegamos o índice da linha do item clicado.
        const int trackIndex = m_trackList->row(item);
        qDebug() << "[PlaylistComponent] Emitindo pedido para remover faixa no índice:" << trackIndex;
        // Emitimos o sinal com o índice da faixa DENTRO desta playlist.
        emit removeTrackRequested(trackIndex);
    });

    contextMenu.exec(m_trackList->mapToGlobal(pos));
}