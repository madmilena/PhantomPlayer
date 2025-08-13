#ifndef PHANTOMPLAYER_LIBRARYTABWIDGET_H
#define PHANTOMPLAYER_LIBRARYTABWIDGET_H

#include <QWidget>
#include <vector>
#include "core/Track.h" // <-- ADICIONADO

class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QMenu;

class LibraryTabWidget : public QWidget {
    Q_OBJECT
public:
    explicit LibraryTabWidget(QWidget* parent = nullptr);
    void updateTrackList(const std::vector<Track>& tracks);

signals:
    void trackDoubleClicked(int trackIndex);
    void addToPlaylistRequested(int trackIndex);
    void searchQueryChanged(const QString& text);

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);
    void showContextMenu(const QPoint& pos);

private:
    QString formatDuration(int totalSeconds);

    QListWidget* m_listWidget;
    QLineEdit* m_searchBar;
    QMenu* m_contextMenu;
};

#endif //PHANTOMPLAYER_LIBRARYTABWIDGET_H