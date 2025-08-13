#ifndef PHANTOMPLAYER_LIBRARYTABWIDGET_H
#define PHANTOMPLAYER_LIBRARYTABWIDGET_H

#include "BaseTab.h"
#include "../../services/SearchService.h"

class QTimer;

class LibraryTabWidget : public BaseTab {
    Q_OBJECT

public:
    explicit LibraryTabWidget(QWidget* parent = nullptr);
    ~LibraryTabWidget() override;

    signals:
        // Sinal para a MainWindow saber qual música adicionar à playlist
        void addToPlaylistRequested(int trackId);

private slots:
    // Sobrescreve o comportamento da BaseTab para fazer a busca online
    void onSearchQueryChanged(const QString& text) override;

    // Slots para a lógica de busca online
    void triggerSearch();
    void onSearchResultsReceived(const std::vector<SearchResult>& results);
    void onSearchFailed(const QString& errorString);

    // Slot para o menu de clique-direito
    void showContextMenu(const QPoint& pos);

private:
    void setupConnections();

    SearchService* m_searchService;
    QTimer* m_searchTimer;
};

#endif //PHANTOMPLAYER_LIBRARYTABWIDGET_H