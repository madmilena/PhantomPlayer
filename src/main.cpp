#include <QApplication>
#include <QStyleFactory>
#include "ui/MainWindow.h"
#include "services/PlaybackService.h"
#include "services/PlaylistManager.h"
#include "core/MediaLibrary.h"

void setFusionStyle(QApplication& app) {
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    setFusionStyle(app);

    // --- CORREÇÃO FINAL AQUI ---

    // 1. Criamos a biblioteca de mídia.
    MediaLibrary mediaLibrary;

    // 2. Criamos o SERVIÇO de playback, passando a biblioteca para ele.
    //    A variável é do tipo PlaybackService.
    PlaybackService playbackService(&mediaLibrary);

    // 3. Criamos o gerenciador de playlists.
    PlaylistManager playlistManager;

    // 4. Criamos a janela principal, passando os ponteiros para os serviços corretos.
    MainWindow mainWindow(&playbackService, &playlistManager);

    mainWindow.show();

    return QApplication::exec();
}