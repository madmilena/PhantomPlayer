#include "SearchService.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

SearchService::SearchService(QObject *parent) : QObject(parent) {
    m_networkManager = new QNetworkAccessManager(this);
}

void SearchService::search(const QString &query) {
    QUrl url("https://musicbrainz.org/ws/2/recording");
    QUrlQuery urlQuery;

    // O MusicBrainz usa a sintaxe Lucene para busca. Podemos especificar os campos.
    QString luceneQuery = QString("recording:%1 OR artist:%1").arg(query);
    urlQuery.addQueryItem("query", luceneQuery);
    urlQuery.addQueryItem("limit", "25"); // Limita a 25 resultados
    urlQuery.addQueryItem("fmt", "json");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);

    // IMPORTANTE: O MusicBrainz exige um User-Agent descritivo.
    // Troque "PhantomPlayer" e o email pelo nome do seu app e seu contato.
    request.setHeader(QNetworkRequest::UserAgentHeader, "PhantomPlayer/1.0 (seu-email@exemplo.com)");

    QNetworkReply* reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onSearchReply(reply);
    });
}

void SearchService::onSearchReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Erro na busca:" << reply->errorString();
        emit searchFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    const QByteArray data = reply->readAll();
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    const QJsonObject rootObj = jsonDoc.object();

    std::vector<SearchResult> results;
    const QJsonArray recordings = rootObj["recordings"].toArray();

    for (const QJsonValue& value : recordings) {
        QJsonObject recordingObj = value.toObject();
        SearchResult result;

        result.trackTitle = recordingObj["title"].toString();

        // Artista geralmente está em 'artist-credit'
        if (recordingObj.contains("artist-credit")) {
            const QJsonArray artistCredits = recordingObj["artist-credit"].toArray();
            if (!artistCredits.isEmpty()) {
                result.artistName = artistCredits[0].toObject()["name"].toString();
            }
        }

        // Precisamos do ID do álbum ('release') para buscar a capa
        if (recordingObj.contains("releases")) {
            const QJsonArray releases = recordingObj["releases"].toArray();
            if (!releases.isEmpty()) {
                result.releaseId = releases[0].toObject()["id"].toString();
                // Montamos o URL da capa usando o ID do release
                result.thumbnailUrl = QString("https://coverartarchive.org/release/%1/front-250").arg(result.releaseId);
            }
        }

        // Só adiciona o resultado se tivermos as informações essenciais
        if (!result.trackTitle.isEmpty() && !result.artistName.isEmpty() && !result.releaseId.isEmpty()) {
            results.push_back(result);
        }
    }

    qDebug() << "Busca concluída. Encontrados" << results.size() << "resultados válidos.";
    emit searchResultsReady(results);
    reply->deleteLater();
}//
// Created by Milena Madsen de Oliveira Silva on 13/08/25.
//