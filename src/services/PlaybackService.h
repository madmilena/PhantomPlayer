#ifndef PHANTOMPLAYER_PLAYBACKSERVICE_H
#define PHANTOMPLAYER_PLAYBACKSERVICE_H

#include <QObject>
#include <SFML/Audio.hpp>
#include <vector>
#include "core/MediaLibrary.h"
#include "core/Track.h"

// O enum estava correto, só precisava ser usado corretamente em outros arquivos.
enum class RepeatMode { None, One, All };

class QTimer; // Forward declaration

class PlaybackService : public QObject {
    Q_OBJECT

public:
    explicit PlaybackService(MediaLibrary* mediaLibrary, QObject *parent = nullptr);
    ~PlaybackService() override;

    [[nodiscard]] const std::vector<Track>& getTracks() const;
    [[nodiscard]] float getInitialVolume() const;
    [[nodiscard]] RepeatMode getRepeatMode() const;
    // Corrigido: definido inline e como const para evitar erros de linkage.
    [[nodiscard]] MediaLibrary* getMediaLibrary() const { return m_mediaLibrary; }

public slots:
    void playTrack(int trackIndex);
    void togglePlayPause();
    void stop();
    void next();
    void prev();
    void seek(float position);
    void setVolume(float volume);
    void setShuffle(bool shuffle);
    void setRepeatMode(RepeatMode mode);

    signals:
        // Corrigido: O sinal só precisa emitir a nova faixa, não o índice.
        void trackChanged(const Track& track);
    void playbackStateChanged(sf::SoundSource::Status status);
    void progressUpdated(float currentTime, float totalDuration);
    void volumeChanged(int volume);

private slots:
    void update();

private:
    void playCurrentTrack();
    // Declarando a função que estava faltando
    void generateShuffleList();

    sf::Music m_music;
    MediaLibrary* m_mediaLibrary;
    int m_currentTrackIndex = -1;
    bool m_isShuffle = false;
    RepeatMode m_repeatMode = RepeatMode::None;
    // Declarando o vetor de índices embaralhados que estava faltando
    std::vector<int> m_shuffledIndices;
    int m_queuePosition = -1;
    QTimer* m_progressTimer;
};

#endif //PHANTOMPLAYER_PLAYBACKSERVICE_H