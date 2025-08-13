#include "BaseTab.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QVariant>
#include <filesystem>
#include "ui/components/SearchBarWidget.h"

namespace fs = std::filesystem;

BaseTab::BaseTab(QWidget *parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    m_searchBar = new SearchBarWidget(this);
    m_listWidget = new QListWidget(this);

    layout->addWidget(m_searchBar);
    layout->addWidget(m_listWidget);
    layout->setContentsMargins(0,0,0,0);

    connect(m_searchBar, &SearchBarWidget::searchQueryChanged, this, &BaseTab::onSearchQueryChanged);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &BaseTab::onItemDoubleClicked);
}

// Implementação correta para mostrar a lista de faixas locais
void BaseTab::updateTrackList(const std::vector<Track>& tracks) {
    m_listWidget->clear();
    for (const auto& track : tracks) {
        QString durationStr = formatDuration(track.durationInSeconds);
        QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
        if (track.artist.empty() || track.title.empty()) {
             displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
        }
        auto* item = new QListWidgetItem(displayText, m_listWidget);
        // Guardamos o ID da faixa (não o índice da linha) para robustez
        item->setData(Qt::UserRole, track.trackId);
    }
}

// Implementação do método que a MainWindow precisa
void BaseTab::clearSearch() {
    if (m_searchBar) {
        m_searchBar->clear();
    }
}

void BaseTab::onItemDoubleClicked(const QListWidgetItem* item) {
    if (item) {
        const int trackId = item->data(Qt::UserRole).toInt();
        emit trackDoubleClicked(trackId);
    }
}

// Implementação SEM 'const' no final, para corresponder ao .h
void BaseTab::onSearchQueryChanged(const QString& text) {
    const QString lowerCaseQuery = text.toLower();
    for (int i = 0; i < m_listWidget->count(); ++i) {
        QListWidgetItem* item = m_listWidget->item(i);
        bool matches = item->text().toLower().contains(lowerCaseQuery);
        item->setHidden(!matches);
    }
}

QString BaseTab::formatDuration(const int totalSeconds) {
    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}