#include "LibraryTabWidget.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QInputDialog>
#include <filesystem>

namespace fs = std::filesystem;

LibraryTabWidget::LibraryTabWidget(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);

    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Buscar na biblioteca...");
    layout->addWidget(m_searchBar);

    m_listWidget = new QListWidget(this);
    layout->addWidget(m_listWidget);

    m_contextMenu = new QMenu(this);

    connect(m_searchBar, &QLineEdit::textChanged, this, &LibraryTabWidget::onSearchTextChanged);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &LibraryTabWidget::onItemDoubleClicked);
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_listWidget, &QWidget::customContextMenuRequested, this, &LibraryTabWidget::showContextMenu);
}

void LibraryTabWidget::setTracks(const std::vector<Track>& tracks) {
    m_tracks = tracks;
    updateTrackList();
}

void LibraryTabWidget::updateTrackList() {
    m_listWidget->clear();
    for (size_t i = 0; i < m_tracks.size(); ++i) {
        const auto& track = m_tracks[i];
        QString durationStr = formatDuration(track.durationInSeconds);
        QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
        if (track.artist.empty() || track.title.empty()) {
            displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
        }
        auto* item = new QListWidgetItem(displayText, m_listWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(static_cast<int>(i)));
    }
}

QString LibraryTabWidget::formatDuration(int totalSeconds) const {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

void LibraryTabWidget::onSearchTextChanged(const QString& text) {
    QString lowerText = text.toLower();
    for (int i = 0; i < m_listWidget->count(); ++i) {
        auto* item = m_listWidget->item(i);
        bool visible = item->text().toLower().contains(lowerText);
        item->setHidden(!visible);
    }
}

void LibraryTabWidget::onItemDoubleClicked(QListWidgetItem* item) {
    int index = item->data(Qt::UserRole).toInt();
    emit trackSelected(index);
}

void LibraryTabWidget::showContextMenu(const QPoint& pos) {
    auto* item = m_listWidget->itemAt(pos);
    if (!item) return;

    m_contextMenu->clear();
    QAction* addToPlaylist = m_contextMenu->addAction("Adicionar à playlist");
    connect(addToPlaylist, &QAction::triggered, this, &LibraryTabWidget::addToPlaylist);

    m_contextMenu->popup(m_listWidget->viewport()->mapToGlobal(pos));
}

void LibraryTabWidget::addToPlaylist() {
    // aqui você pode emitir um sinal, ou abrir um diálogo, etc
    emit createNewPlaylistRequested();
}
