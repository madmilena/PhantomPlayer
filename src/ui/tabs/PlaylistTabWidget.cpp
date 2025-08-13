#include "PlaylistTabWidget.h"
#include "BaseTab.h"
#include <QListWidget>

PlaylistTabWidget::PlaylistTabWidget(QWidget *parent) : QTabWidget(parent) {
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, &PlaylistTabWidget::playlistClosed);
}

void PlaylistTabWidget::updatePlaylists(const std::vector<Playlist>& playlists, const std::vector<Track>& allTracks) {
    while (count() > playlists.size()) {
        delete widget(count() - 1);
        removeTab(count() - 1);
    }

    for (int i = 0; i < playlists.size(); ++i) {
        BaseTab* playlistTab;
        if (i < count()) {
            playlistTab = qobject_cast<BaseTab*>(widget(i));
            setTabText(i, playlists[i].name);
        } else {
            playlistTab = new BaseTab(this);
            addTab(playlistTab, playlists[i].name);
            connect(playlistTab, &BaseTab::trackDoubleClicked, this, &PlaylistTabWidget::trackDoubleClicked);
        }
        
        playlistTab->updateTrackList(playlists[i].trackIndices, allTracks);
    }
}