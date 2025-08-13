#include "LibraryTabWidget.h"
#include "ui/components/SearchBarWidget.h"
#include "services/SearchService.h"
#include <QListWidget>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMenu>
#include <QDebug>

LibraryTabWidget::LibraryTabWidget(QWidget* parent)
    : BaseTab(parent)
{
    m_searchService = new SearchService(this);
    m_searchTimer = new QTimer(this);

    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(400);

    // Habilita a política de menu de contexto para a lista
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    setupConnections();
}

LibraryTabWidget::~LibraryTabWidget() {}

void LibraryTabWidget::setupConnections() {
    // A conexão da busca já é feita na BaseTab. Como onSearchQueryChanged é virtual,
    // a versão desta classe será chamada automaticamente.

    connect(m_searchTimer, &QTimer::timeout, this, &LibraryTabWidget::triggerSearch);
    connect(m_searchService, &SearchService::searchResultsReady, this, &LibraryTabWidget::onSearchResultsReceived);
    connect(m_searchService, &SearchService::searchFailed, this, &LibraryTabWidget::onSearchFailed);

    // Conexão para o menu de clique-direito
    connect(m_listWidget, &QListWidget::customContextMenuRequested, this, &LibraryTabWidget::showContextMenu);
}

void LibraryTabWidget::onSearchQueryChanged(const QString& text) {
    if (text.trimmed().length() < 3) {
        m_searchTimer->stop();
        // Aqui você poderia recarregar a lista local de musicas, se quisesse
        // updateTrackList(m_localTracks);
        return;
    }
    m_searchTimer->start();
}

void LibraryTabWidget::triggerSearch() {
    QString query = m_searchBar->text().trimmed();
    qDebug() << "Disparando busca online por:" << query;
    m_listWidget->clear();
    m_listWidget->addItem("Buscando...");
    m_searchService->search(query);
}

void LibraryTabWidget::onSearchResultsReceived(const std::vector<SearchResult>& results) {
    m_listWidget->clear();

    if (results.empty()) {
        m_listWidget->addItem("Nenhum resultado encontrado.");
        return;
    }

    auto* imageDownloader = new QNetworkAccessManager();

    for (const auto& result : results) {
        auto* item = new QListWidgetItem(result.artistName + " - " + result.trackTitle, m_listWidget);
        // O ideal seria que a API retornasse um ID para a faixa, que guardaríamos aqui:
        // item->setData(Qt::UserRole, result.trackApiId);

        if (!result.thumbnailUrl.isEmpty()) {
            QNetworkRequest request(result.thumbnailUrl);
            QNetworkReply* reply = imageDownloader->get(request);
            connect(reply, &QNetworkReply::finished, this, [item, reply]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray data = reply->readAll();
                    QPixmap pixmap;
                    if (pixmap.loadFromData(data)) {
                        item->setIcon(QIcon(pixmap));
                    }
                }
                reply->deleteLater();
            });
        }
    }
    connect(imageDownloader, &QObject::destroyed, imageDownloader, &QObject::deleteLater);
}

void LibraryTabWidget::onSearchFailed(const QString& errorString) {
    m_listWidget->clear();
    m_listWidget->addItem("Erro na busca: " + errorString);
}

// Implementação do menu de contexto que a MainWindow precisa
void LibraryTabWidget::showContextMenu(const QPoint& pos) {
    QListWidgetItem* item = m_listWidget->itemAt(pos);
    if (!item) {
        return;
    }

    // Não mostra o menu para itens de status como "Buscando..."
    if (item->data(Qt::UserRole).isNull()) {
        return;
    }

    QMenu contextMenu(this);
    QAction* addAction = contextMenu.addAction("Adicionar à Playlist");

    connect(addAction, &QAction::triggered, this, [this, item]() {
        bool ok;
        int trackId = item->data(Qt::UserRole).toInt(&ok);
        if (ok) {
            emit addToPlaylistRequested(trackId);
        }
    });

    contextMenu.exec(m_listWidget->mapToGlobal(pos));
}