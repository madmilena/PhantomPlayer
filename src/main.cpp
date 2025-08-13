#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "ui/MainWindow.h"
#include "services/PlaybackService.h"
#include "services/PlaylistManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile file(":/style.qss");
    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        app.setStyleSheet(stream.readAll());
    }

    // --- LÓGICA DE CRIAÇÃO CORRIGIDA ---
    PlaybackService playbackService;
    PlaylistManager playlistManager(playbackService.getMediaLibrary()); // 1. Pegamos a MediaLibrary do serviço principal
                                                                       // 2. Passamos para o PlaylistManager
    MainWindow window(&playbackService, &playlistManager);
    // ------------------------------------

    window.show();

    return app.exec();
}