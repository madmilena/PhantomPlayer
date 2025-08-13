#include "PlaylistTabWidget.h"
#include <QListWidget>
#include <QMenu>
#include <filesystem>

namespace fs = std::filesystem;

PlaylistTabWidget::PlaylistTabWidget(QWidget* parent) : QTabWidget(parent) {
    connect(this, &QTabWidget::currentChanged, this, [this](int index){
        if(index >= 0 && index < count()) {
            updatePlaylistTab(index);
        }
    });
}

void PlaylistTabWidget::setPlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks) {
    m_playlists = playlists;
    m_allTracks = allTracks;

    // Remove abas antigas
    while (count() > 0) {
        QWidget* w = widget(0);
        removeTab(0);
        delete w;
    }

    // Adiciona abas novas
    for (const auto& playlist : m_playlists) {
        auto* listWidget = new QListWidget(this);
        addTab(listWidget, playlist.name);

        listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(listWidget, &QListWidget::itemDoubleClicked, this, &PlaylistTabWidget::onItemDoubleClicked);
        connect(listWidget, &QWidget::customContextMenuRequested, this, &PlaylistTabWidget::showContextMenu);
    }

    // Atualiza a aba atual
    updatePlaylistTab(currentIndex());
}

void PlaylistTabWidget::updatePlaylistTab(int tabIndex) {
    if (tabIndex < 0 || tabIndex >= count()) return;

    QListWidget* listWidget = qobject_cast<QListWidget*>(widget(tabIndex));
    if (!listWidget) return;

    listWidget->clear();

    const auto& playlist = m_playlists[tabIndex];
    for (int trackIndex : playlist.trackIndices) {
        if(trackIndex < 0 || trackIndex >= static_cast<int>(m_allTracks.size())) continue;
        const auto& track = m_allTracks[trackIndex];
        QString durationStr = QString("%1:%2")
            .arg(track.durationInSeconds / 60, 2, 10, QChar('0'))
            .arg(track.durationInSeconds % 60, 2, 10, QChar('0'));

        QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
        if (track.artist.empty() || track.title.empty()) {
            displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
        }
        auto* item = new QListWidgetItem(displayText, listWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(trackIndex));
    }
}

void PlaylistTabWidget::onItemDoubleClicked(QListWidgetItem* item) {
    int index = item->data(Qt::UserRole).toInt();
    emit trackSelected(index);
}

void PlaylistTabWidget::showContextMenu(const QPoint& pos) {
    QListWidget* listWidget = qobject_cast<QListWidget*>(currentWidget());
    if (!listWidget) return;

    auto* item = listWidget->itemAt(pos);
    if (!item) return;

    QMenu menu(this);
    menu.addAction("Remover da playlist");
    // aqui pode adicionar mais ações

    menu.exec(listWidget->viewport()->mapToGlobal(pos));
}
