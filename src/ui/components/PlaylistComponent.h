#ifndef PHANTOMPLAYER_PLAYLISTCOMPONENT_H
#define PHANTOMPLAYER_PLAYLISTCOMPONENT_H

#include <QWidget>
#include <vector>
#include "core/Track.h"

// Forward declarations para compilação mais rápida
class QListWidget;
class QListWidgetItem;

class PlaylistComponent final : public QWidget {
    Q_OBJECT

    public:
    explicit PlaylistComponent(int playlistId, QWidget *parent = nullptr);
    ~PlaylistComponent() override = default; // Usar default é uma boa prática

    void updateTracks(const std::vector<int>& trackIds, const std::vector<Track>& allTracks);

    signals:
        void trackDoubleClicked(QListWidgetItem *item);
    void removeTrackRequested(int trackIndex);

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onItemDoubleClicked(QListWidgetItem *item);

    // --- O NOVO SLOT PARA A AÇÃO DE REMOVER ---
    void onRemoveTrackTriggered();

private:
    const int m_playlistId;
    QListWidget* m_trackList;
};

#endif //PHANTOMPLAYER_PLAYLISTCOMPONENT_H