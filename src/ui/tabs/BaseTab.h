#ifndef PHANTOMPLAYER_BASETAB_H
#define PHANTOMPLAYER_BASETAB_H

#include <QWidget>
#include <vector>
#include <filesystem>
#include "core/Track.h"

class QListWidget;
class QListWidgetItem;
class SearchBarWidget;

namespace fs = std::filesystem;

class BaseTab : public QWidget {
    Q_OBJECT

public:
    explicit BaseTab(QWidget* parent = nullptr);
    void updateTrackList(const std::vector<int>& trackIndices, const std::vector<Track>& allTracks);

signals:
    void trackDoubleClicked(int trackIndex);

protected slots:
    void onItemDoubleClicked(QListWidgetItem* item);
    void onSearchQueryChanged(const QString& text);

protected:
    QString formatDuration(int totalSeconds);

    QListWidget* m_listWidget;
    SearchBarWidget* m_searchBar;
};

#endif //PHANTOMPLAYER_BASETAB_H