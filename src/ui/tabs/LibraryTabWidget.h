#ifndef PHANTOMPLAYER_LIBRARYTABWIDGET_H
#define PHANTOMPLAYER_LIBRARYTABWIDGET_H

#include "BaseTab.h"
#include <vector>
#include "core/Track.h" // Incluído

class QListWidgetItem;
class QMenu;

class LibraryTabWidget : public BaseTab {
    Q_OBJECT

public:
    explicit LibraryTabWidget(QWidget* parent = nullptr);
    void updateTrackList(const std::vector<Track>& tracks);

signals:
    void addToPlaylistRequested(int trackIndex);

private slots:
    void showContextMenu(const QPoint& pos);
    void onAddToPlaylist();

private:
    QMenu* m_contextMenu;
};

#endif //PHANTOMPLAYER_LIBRARYTABWIDGET_H