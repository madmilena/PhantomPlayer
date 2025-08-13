#ifndef PHANTOMPLAYER_PLAYLISTCOMPONENT_H
#define PHANTOMPLAYER_PLAYLISTCOMPONENT_H

#include <QWidget>
#include <QListWidgetItem>
#include <vector>
#include "core/Track.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PlaylistComponent; }
QT_END_NAMESPACE

class PlaylistComponent final : public QWidget {
    Q_OBJECT

    public:
    explicit PlaylistComponent(int playlistId, QWidget *parent = nullptr);
    ~PlaylistComponent() override;

    void updateTracks(const std::vector<Track>& tracks);

    signals:
        void trackDoubleClicked(QListWidgetItem *item);
    void removeTrackRequested(int trackIndex);

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onItemDoubleClicked(QListWidgetItem *item);

    // --- NOVO SLOT DECLARADO AQUI ---
    void onRemoveTrackRequested();

private:
    Ui::PlaylistComponent *ui{};
    const int m_playlistId;
};

#endif //PHANTOMPLAYER_PLAYLISTCOMPONENT_H