#include "PlaylistTabWidget.h"
#include <QListWidget>

QString PlaylistTabWidget::formatDuration(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

PlaylistTabWidget::PlaylistTabWidget(QWidget *parent) : QTabWidget(parent) {
}

void PlaylistTabWidget::updatePlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks) {
    // Remove abas que não existem mais
    while (count() > playlists.size()) {
        removeTab(count() - 1);
    }

    // Adiciona ou atualiza as abas existentes
    for (int i = 0; i < playlists.size(); ++i) {
        QListWidget* playlistWidget;
        if (i < count()) {
            playlistWidget = qobject_cast<QListWidget*>(widget(i));
            setTabText(i, playlists[i].name);
        } else {
            playlistWidget = new QListWidget(this);
            addTab(playlistWidget, playlists[i].name);
            connect(playlistWidget, &QListWidget::itemDoubleClicked, this, &PlaylistTabWidget::onItemDoubleClicked);
        }

        playlistWidget->clear();
        for (int trackIndex : playlists[i].trackIndices) {
            if (trackIndex >= 0 && trackIndex < allTracks.size()) {
                const auto& track = allTracks[trackIndex];
                QString durationStr = formatDuration(track.durationInSeconds);
                QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
                if (track.artist.empty() || track.title.empty()) {
                     displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
                }
                auto* item = new QListWidgetItem(displayText, playlistWidget);
                item->setData(Qt::UserRole, QVariant::fromValue(trackIndex));
            }
        }
    }
}

void PlaylistTabWidget::onItemDoubleClicked(QListWidgetItem* item) {
    if (item) {
        int trackIndex = item->data(Qt::UserRole).toInt();
        emit trackDoubleClicked(trackIndex);
    }
}