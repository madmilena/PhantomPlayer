#ifndef PHANTOMPLAYER_SEARCHSERVICE_H
#define PHANTOMPLAYER_SEARCHSERVICE_H

#include <QObject>
#include <QString>
#include <vector>

// Struct para transportar os dados de forma limpa entre as classes
struct SearchResult {
    QString trackTitle;
    QString artistName;
    QString releaseId; // O ID do álbum no MusicBrainz (MBID)
    QString thumbnailUrl; // O URL completo para a capa
};

class QNetworkAccessManager;
class QNetworkReply;

class SearchService : public QObject {
    Q_OBJECT
public:
    explicit SearchService(QObject *parent = nullptr);

public slots:
    void search(const QString& query);

    signals:
        void searchResultsReady(const std::vector<SearchResult>& results);
    void searchFailed(const QString& errorString);

private slots:
    void onSearchReply(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_networkManager;
};

#endif //PHANTOMPLAYER_SEARCHSERVICE_H