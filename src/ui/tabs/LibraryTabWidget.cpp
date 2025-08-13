#include "LibraryTabWidget.h"
#include <QListWidget>
#include <QMenu>
#include <QVariant>
#include <numeric>
#include "../components/SearchBarWidget.h"

LibraryTabWidget::LibraryTabWidget(QWidget *parent) : BaseTab(parent) {
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_listWidget, &QWidget::customContextMenuRequested, this, &LibraryTabWidget::showContextMenu);
}

void LibraryTabWidget::updateTrackList(const std::vector<Track>& tracks) {
    std::vector<int> indices(tracks.size());
    std::iota(indices.begin(), indices.end(), 0);
    BaseTab::updateTrackList(indices, tracks);
}

void LibraryTabWidget::showContextMenu(const QPoint& pos) {
    if (const QListWidgetItem* item = m_listWidget->itemAt(pos); !item) return;

    m_contextMenu = new QMenu(this);
    const QAction* addToPlaylistAction = m_contextMenu->addAction("Adicionar à playlist...");
    connect(addToPlaylistAction, &QAction::triggered, this, &LibraryTabWidget::onAddToPlaylist);
    
    m_contextMenu->popup(m_listWidget->viewport()->mapToGlobal(pos));
}

void LibraryTabWidget::onAddToPlaylist() {
    if (auto selectedItems = m_listWidget->selectedItems(); !selectedItems.empty()) {
        const int trackIndex = selectedItems.first()->data(Qt::UserRole).toInt();
        emit addToPlaylistRequested(trackIndex);
    }
}

void LibraryTabWidget::clearSearch() {
    if (m_searchBar) {
        m_searchBar->clear();
    }
}