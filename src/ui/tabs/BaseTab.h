#ifndef PHANTOMPLAYER_BASETAB_H
#define PHANTOMPLAYER_BASETAB_H

#include <QWidget>
#include <vector>
#include "core/Track.h"

class QListWidget;
class QListWidgetItem;
class SearchBarWidget;

class BaseTab : public QWidget {
    Q_OBJECT

public:
    explicit BaseTab(QWidget* parent = nullptr);
    // Assinatura correta para receber a lista de faixas locais
    void updateTrackList(const std::vector<Track>& tracks);
    // Método para a MainWindow poder limpar a busca ao trocar de aba
    void clearSearch();

protected slots:
    // Tornamos virtual para que a LibraryTabWidget possa sobrescrevê-lo
    virtual void onSearchQueryChanged(const QString& text);
    // Garante que o duplo clique emita o ID correto da faixa
    void onItemDoubleClicked(const QListWidgetItem* item);

    signals:
        // O sinal emite o ID da faixa, que é mais robusto que o índice da linha
        void trackDoubleClicked(int trackId);

protected:
    static QString formatDuration(int totalSeconds);
    QListWidget* m_listWidget;
    SearchBarWidget* m_searchBar;
};

#endif //PHANTOMPLAYER_BASETAB_H