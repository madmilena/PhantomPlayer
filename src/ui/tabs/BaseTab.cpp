#include "BaseTab.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QVariant>
#include "ui/components/SearchBarWidget.h"

#include "BaseTab.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QVariant>
#include "ui/components/SearchBarWidget.h"

BaseTab::BaseTab(QWidget *parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    m_searchBar = new SearchBarWidget(this);
    m_listWidget = new QListWidget(this);

    layout->addWidget(m_searchBar);
    layout->addWidget(m_listWidget);

    // CORREÇÃO: Conecta ao sinal correto que acabamos de adicionar
    connect(m_searchBar, &SearchBarWidget::searchQueryChanged, this, &BaseTab::onSearchQueryChanged);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &BaseTab::onItemDoubleClicked);
}

void BaseTab::updateTrackList(const std::vector<int>& trackIndices, const std::vector<Track>& allTracks) {
    m_listWidget->clear();
    for (int trackIndex : trackIndices) {
        if (trackIndex >= 0 && trackIndex < allTracks.size()) {
            const auto& track = allTracks[trackIndex];
            QString durationStr = formatDuration(track.durationInSeconds);
            QString displayText = QString::fromStdString(track.artist + " - " + track.title + "\t" + durationStr.toStdString());
            if (track.artist.empty() || track.title.empty()) {
                 displayText = QString::fromStdString(fs::path(track.filePath).stem().string() + "\t" + durationStr.toStdString());
            }
            auto* item = new QListWidgetItem(displayText, m_listWidget);
            item->setData(Qt::UserRole, QVariant::fromValue(trackIndex)); // Sempre guarda o índice original
        }
    }
}

void BaseTab::onItemDoubleClicked(QListWidgetItem* item) {
    if (item) {
        int trackIndex = item->data(Qt::UserRole).toInt();
        emit trackDoubleClicked(trackIndex);
    }
}

void BaseTab::onSearchQueryChanged(const QString& text) {
    QString lowerCaseQuery = text.toLower();
    for (int i = 0; i < m_listWidget->count(); ++i) {
        QListWidgetItem* item = m_listWidget->item(i);
        bool matches = item->text().toLower().contains(lowerCaseQuery);
        item->setHidden(!matches);
    }
}

QString BaseTab::formatDuration(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}