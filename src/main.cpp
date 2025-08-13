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

    MediaLibrary mediaLibrary;
    MediaLibrary* playbackService(&mediaLibrary);

    // --- CORREÇÃO AQUI ---
    // Criamos o PlaylistManager usando seu construtor padrão, sem argumentos.
    // Ele não precisa mais da MediaLibrary no construtor.
    PlaylistManager playlistManager;

    MainWindow mainWindow(&playbackService, &playlistManager);
    mainWindow.show();

    return QApplication::exec();
}