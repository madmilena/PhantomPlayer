#ifndef PHANTOMPLAYER_BASETAB_H
#define PHANTOMPLAYER_BASETAB_H

#include <QWidget>
#include <vector>
#include <filesystem>
#include "core/Track.h"

class QListWidget;
class QListWidgetItem;
class SearchBarWidget;

namespace fs = std::filesystem;

class BaseTab : public QWidget {
    Q_OBJECT

public:
    explicit BaseTab(QWidget* parent = nullptr);
    // Este método é ótimo para mostrar as faixas locais
    void updateTrackList(const std::vector<Track>& allTracks);

protected slots:
    // Permite que classes filhas reimplementem a lógica de busca
    virtual void onSearchQueryChanged(const QString& text);
    void onItemDoubleClicked(const QListWidgetItem* item);

    signals:
        // O sinal deve emitir o ID da faixa, não o índice da lista
        void trackDoubleClicked(int trackId);

protected:
    static QString formatDuration(int totalSeconds);

    QListWidget* m_listWidget;
    SearchBarWidget* m_searchBar;
};

#endif //PHANTOMPLAYER_BASETAB_H