#include "PlaylistTabWidget.h"

#include <qlistwidget.h>
#include <QMenu>

#include "BaseTab.h"

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
            
            // --- NOVA LÓGICA ---
            // Habilita o menu de contexto para esta nova aba de playlist
            playlistTab->getListWidget()->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(playlistTab->getListWidget(), &QWidget::customContextMenuRequested, this, [this, i, playlistTab]{
                QListWidgetItem* item = playlistTab->getListWidget()->currentItem();
                if(!item) return;

                QMenu contextMenu(this);
                const QAction* removeAction = contextMenu.addAction("Remover da Playlist");
                
                connect(removeAction, &QAction::triggered, this, [this, i, item]{
                    const int trackIndex = item->data(Qt::UserRole).toInt();
                    emit removeTrackFromPlaylist(i, trackIndex); // Emite o sinal com o índice da playlist e da faixa
                });
                
                contextMenu.exec(QCursor::pos());
            });
            // ---------------------
        }
        
        playlistTab->updateTrackList(playlists[i].trackIndices, allTracks);
    }
}
