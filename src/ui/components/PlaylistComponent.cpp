#include "PlaylistComponent.h"
#include <QMenu>
#include <QDebug> // Inclua para usar qDebug

PlaylistComponent::PlaylistComponent(int playlistId, QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaylistComponent), m_playlistId(playlistId)
{
    ui->setupUi(this);
    ui->trackList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->trackList, &QListWidget::customContextMenuRequested, this, &PlaylistComponent::onCustomContextMenuRequested);
    connect(ui->trackList, &QListWidget::itemDoubleClicked, this, &PlaylistComponent::onItemDoubleClicked);
}

PlaylistComponent::~PlaylistComponent() {
    delete ui;
}

void PlaylistComponent::updateTracks(const std::vector<Track>& tracks) {
    ui->trackList->clear();
    for (const auto& track : tracks) {
        auto* item = new QListWidgetItem(QString::fromStdString(track.getTitle()));
        item->setData(Qt::UserRole, track.getId());
        ui->trackList->addItem(item);
    }
}

void PlaylistComponent::onCustomContextMenuRequested(const QPoint &pos) {
    // Garantir que um item esteja selecionado antes de mostrar o menu
    if (ui->trackList->itemAt(pos) == nullptr) {
        return;
    }

    QMenu contextMenu(this);
    QAction* removeAction = contextMenu.addAction("Remover da Playlist"); // Criamos a ação

    // --- A LINHA QUE FALTAVA ESTÁ AQUI ---
    // Conecta o clique na ação "removeAction" ao nosso novo slot.
    connect(removeAction, &QAction::triggered, this, &PlaylistComponent::onRemoveTrackRequested);

    contextMenu.exec(ui->trackList->mapToGlobal(pos));
}

// --- IMPLEMENTAÇÃO DO NOVO SLOT ---
void PlaylistComponent::onRemoveTrackRequested() {
    QListWidgetItem* currentItem = ui->trackList->currentItem();
    if (currentItem) {
        const int trackIndex = ui->trackList->row(currentItem);
        qDebug() << "[PASSO 1 - PlaylistComponent] Ação 'Remover' foi clicada! Emitindo sinal para remover a faixa no índice:" << trackIndex;
        emit removeTrackRequested(trackIndex);
    }
}

void PlaylistComponent::onItemDoubleClicked(QListWidgetItem *item) {
    emit trackDoubleClicked(item);
}