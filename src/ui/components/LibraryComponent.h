#ifndef PHANTOMPLAYER_LIBRARYCOMPONENT_H
#define PHANTOMPLAYER_LIBRARYCOMPONENT_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <vector>
#include "../core/Track.h"

class LibraryComponent : public QWidget {
    Q_OBJECT
public:
    explicit LibraryComponent(QWidget* parent = nullptr);

    void setTracks(const std::vector<Track>& tracks);
    void filterTracks(const QString& query);

signals:
    void trackDoubleClicked(Track track);

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);
    void onSearchTextChanged(const QString& text);

private:
    void refreshList();

    QLineEdit* m_searchBar;
    QListWidget* m_trackList;
    std::vector<Track> m_allTracks;
    std::vector<Track> m_filteredTracks;
};

#endif // PHANTOMPLAYER_LIBRARYCOMPONENT_H
