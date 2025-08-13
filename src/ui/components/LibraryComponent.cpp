#include "LibraryComponent.h"
#include <QVBoxLayout>
#include <QListWidgetItem>

LibraryComponent::LibraryComponent(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Buscar na biblioteca...");

    m_trackList = new QListWidget(this);

    layout->addWidget(m_searchBar);
    layout->addWidget(m_trackList);

    connect(m_searchBar, &QLineEdit::textChanged, this, &LibraryComponent::onSearchTextChanged);
    connect(m_trackList, &QListWidget::itemDoubleClicked, this, &LibraryComponent::onItemDoubleClicked);
}

void LibraryComponent::setTracks(const std::vector<Track>& tracks) {
    m_allTracks = tracks;
    m_filteredTracks = tracks;
    refreshList();
}

void LibraryComponent::filterTracks(const QString& query) {
    if (query.isEmpty()) {
        m_filteredTracks = m_allTracks;
    } else {
        m_filteredTracks.clear();
        for (const auto& track : m_allTracks) {
            if (track.title.contains(query, Qt::CaseInsensitive) ||
                track.artist.contains(query, Qt::CaseInsensitive)) {
                m_filteredTracks.push_back(track);
            }
        }
    }
    refreshList();
}

void LibraryComponent::refreshList() {
    m_trackList->clear();
    for (const auto& track : m_filteredTracks) {
        auto* item = new QListWidgetItem(QString("%1 - %2").arg(track.artist, track.title));
        // Guardar ponteiro para Track, pode usar QVariant para guardar o índice ou dados adicionais
        item->setData(Qt::UserRole, QVariant::fromValue(track.id)); 
        m_trackList->addItem(item);
    }
}

void LibraryComponent::onItemDoubleClicked(QListWidgetItem* item) {
    if (!item) return;
    // Pegar o id salvo e buscar o Track correspondente
    int trackId = item->data(Qt::UserRole).toInt();
    for (const auto& track : m_filteredTracks) {
        if (track.id == trackId) {
            emit trackDoubleClicked(track);
            break;
        }
    }
}

void LibraryComponent::onSearchTextChanged(const QString& text) {
    filterTracks(text);
}
