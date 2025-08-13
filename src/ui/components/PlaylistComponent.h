#ifndef PHANTOMPLAYER_PLAYLISTCOMPONENT_H
#define PHANTOMPLAYER_PLAYLISTCOMPONENT_H

#include <QWidget>
#include <vector>
#include "core/Track.h"

class QListWidget;
class QListWidgetItem;

// Herda diretamente de QWidget para maior simplicidade e clareza.
class PlaylistComponent final : public QWidget {
    Q_OBJECT

    public:
    explicit PlaylistComponent(QWidget *parent = nullptr);
    ~PlaylistComponent() override;

    // Método específico para atualizar esta lista com as faixas corretas.
    void updateTrackList(const std::vector<Track>& tracks);

    signals:
        // Sinais que a PlaylistTabWidget vai ouvir.
        void trackDoubleClicked(QListWidgetItem *item);
    void removeTrackRequested(int trackIndex);

private slots:
    // Slots internos para lidar com eventos da QListWidget.
    void onItemDoubleClicked(QListWidgetItem *item);
    void onCustomContextMenuRequested(const QPoint &pos);

private:
    QListWidget* m_trackList;
};

#endif //PHANTOMPLAYER_PLAYLISTCOMPONENT_H