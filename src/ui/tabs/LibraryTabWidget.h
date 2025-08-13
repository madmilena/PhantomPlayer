#ifndef PHANTOMPLAYER_LIBRARYTABWIDGET_H
#define PHANTOMPLAYER_LIBRARYTABWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QMenu>

class LibraryTabWidget : public QWidget {
    Q_OBJECT
public:
    explicit LibraryTabWidget(QWidget* parent = nullptr);

    void setTracks(const std::vector<Track>& tracks);

signals:
    void trackSelected(int index);
    void createNewPlaylistRequested();

private slots:
    void onSearchTextChanged(const QString& text);
    void onItemDoubleClicked(QListWidgetItem* item);
    void showContextMenu(const QPoint& pos);
    void addToPlaylist();

private:
    void updateTrackList();
    QString formatDuration(int totalSeconds) const;

    QLineEdit* m_searchBar;
    QListWidget* m_listWidget;
    QMenu* m_contextMenu;

    std::vector<Track> m_tracks;
};

#endif // PHANTOMPLAYER_LIBRARYTABWIDGET_H
