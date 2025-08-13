#include "LibraryTabWidget.h"
#include <QListWidget>
#include <QMenu>
#include <QVBoxLayout>
#include <QVariant>

LibraryTabWidget::LibraryTabWidget(QWidget *parent) : BaseTab(parent) {
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_listWidget, &QWidget::customContextMenuRequested, this, &LibraryTabWidget::showContextMenu);
}

void LibraryTabWidget::updateTrackList(const std::vector<Track>& tracks) {
    m_listWidget->clear();
    for (int i = 0; i < tracks.size(); ++i) {
        const auto& track = tracks[i];
        QString durationStr = formatDuration(track.durationInSeconds);
        QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
        if (track.artist.empty() || track.title.empty()) {
             displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
        }
        auto* item = new QListWidgetItem(displayText, m_listWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(i));
    }
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