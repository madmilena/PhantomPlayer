#include "LibraryTabWidget.h"
#include "ui/components/SearchBarWidget.h"
#include "services/SearchService.h"
#include <QListWidget>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

LibraryTabWidget::LibraryTabWidget(QWidget* parent)
    : BaseTab(parent) // Chama o construtor da classe pai, que monta a UI básica
{
    // 1. Inicializa os componentes específicos da busca online
    m_searchService = new SearchService(this);
    m_searchTimer = new QTimer(this);

    // 2. Configura o timer para a busca "inteligente" (debouncing)
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(400); // 400ms de espera após o usuário parar de digitar

    // 3. Conecta os sinais e slots
    setupConnections();
}

LibraryTabWidget::~LibraryTabWidget() {
    // O Qt cuida de deletar m_searchService e m_searchTimer porque demos 'this' no construtor
}

void LibraryTabWidget::setupConnections() {
    // A conexão da barra de busca agora aponta para o nosso método sobrescrito
    // (A conexão original da BaseTab já faz isso automaticamente por causa do 'virtual')

    // Conecta o fim do timer ao método que efetivamente dispara a busca
    connect(m_searchTimer, &QTimer::timeout, this, &LibraryTabWidget::triggerSearch);

    // Conecta os sinais do nosso serviço aos métodos que tratarão os resultados
    connect(m_searchService, &SearchService::searchResultsReady, this, &LibraryTabWidget::onSearchResultsReceived);
    connect(m_searchService, &SearchService::searchFailed, this, &LibraryTabWidget::onSearchFailed);
}

// Este método SOBRESCRVE o da BaseTab. Em vez de filtrar, ele aciona o timer.
void LibraryTabWidget::onSearchQueryChanged(const QString& text) {
    if (text.trimmed().length() < 3) {
        // Se a busca for curta, não faz nada ou volta para a lista local
        m_searchTimer->stop();
        // updateTrackList(m_localTracks); // Opcional: recarregar a lista local
        return;
    }
    // A cada letra digitada, o timer é reiniciado
    m_searchTimer->start();
}

// Quando o timer finalmente dispara, este método é chamado
void LibraryTabWidget::triggerSearch() {
    QString query = m_searchBar->text().trimmed();
    qDebug() << "Disparando busca online por:" << query;
    m_listWidget->clear(); // Limpa a lista para mostrar que algo está acontecendo
    m_listWidget->addItem("Buscando..."); // Feedback para o usuário
    m_searchService->search(query);
}

// Quando os resultados da API chegam, este slot é ativado
void LibraryTabWidget::onSearchResultsReceived(const std::vector<SearchResult>& results) {
    m_listWidget->clear(); // Limpa a mensagem "Buscando..."

    if (results.empty()) {
        m_listWidget->addItem("Nenhum resultado encontrado.");
        return;
    }

    auto* imageDownloader = new QNetworkAccessManager();

    for (const auto& result : results) {
        auto* item = new QListWidgetItem(result.artistName + " - " + result.trackTitle, m_listWidget);
        // Você pode guardar mais dados aqui se precisar, por exemplo, o ID da música
        // item->setData(Qt::UserRole, result.trackIdFromApi);

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