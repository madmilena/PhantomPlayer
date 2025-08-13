#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QtCore/qfile.h>

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

    // Criação e injeção de dependências corrigida
    PlaybackService playbackService;
    PlaylistManager playlistManager(playbackService.getMediaLibrary());
    MainWindow window(&playbackService, &playlistManager);

    window.show();

    return app.exec();
}