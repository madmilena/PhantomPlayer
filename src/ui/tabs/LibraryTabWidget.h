#ifndef PHANTOMPLAYER_LIBRARYTABWIDGET_H
#define PHANTOMPLAYER_LIBRARYTABWIDGET_H

#include "BaseTab.h"
#include "services/SearchService.h" // Incluindo nosso novo serviço

class QTimer;

class LibraryTabWidget : public BaseTab {
    Q_OBJECT

public:
    explicit LibraryTabWidget(QWidget* parent = nullptr);
    ~LibraryTabWidget() override;

private slots:
    // Sobrescreve o comportamento da BaseTab
    void onSearchQueryChanged(const QString& text) override;

    // Slots específicos para a busca online
    void triggerSearch();
    void onSearchResultsReceived(const std::vector<SearchResult>& results);
    void onSearchFailed(const QString& errorString);

private:
    void setupConnections();

    SearchService* m_searchService;
    QTimer* m_searchTimer;
};

#endif //PHANTOMPLAYER_LIBRARYTABWIDGET_H