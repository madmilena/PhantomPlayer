#include "LibraryTabWidget.h"
#include <QListWidget>
#include <QMenu>
#include <QVariant>
#include <numeric>

LibraryTabWidget::LibraryTabWidget(QWidget *parent) : BaseTab(parent) {
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_listWidget, &QWidget::customContextMenuRequested, this, &LibraryTabWidget::showContextMenu);
}

void LibraryTabWidget::updateTrackList(const std::vector<Track>& tracks) {
    std::vector<int> indices(tracks.size());
    std::iota(indices.begin(), indices.end(), 0); // Cria uma lista de 0, 1, 2, ...
    BaseTab::updateTrackList(indices, tracks);
}

void LibraryTabWidget::showContextMenu(const QPoint& pos) {
    QListWidgetItem* item = m_listWidget->itemAt(pos);
    if (!item) return;

    m_contextMenu = new QMenu(this);
    QAction* addToPlaylistAction = m_contextMenu->addAction("Adicionar à playlist...");
    connect(addToPlaylistAction, &QAction::triggered, this, &LibraryTabWidget::onAddToPlaylist);
    
    m_contextMenu->popup(m_listWidget->viewport()->mapToGlobal(pos));
}

void LibraryTabWidget::onAddToPlaylist() {
    auto selectedItems = m_listWidget->selectedItems();
    if (!selectedItems.empty()) {
        int trackIndex = selectedItems.first()->data(Qt::UserRole).toInt();
        emit addToPlaylistRequested(trackIndex);
    }
}