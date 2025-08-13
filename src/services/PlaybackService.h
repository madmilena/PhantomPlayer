#ifndef SPOTIFYCLONE_PLAYBACKSERVICE_H
#define SPOTIFYCLONE_PLAYBACKSERVICE_H

#include <QObject>
#include <vector>
#include "core/AudioEngine.h"
#include "core/MediaLibrary.h"

enum class RepeatMode {
    None,
    RepeatAll,
    RepeatOne
};

class PlaybackService : public QObject {
    Q_OBJECT

public:
    explicit PlaybackService(QObject *parent = nullptr);

    const std::vector<Track>& getTracks() const;
    float getInitialVolume() const;
    MediaLibrary* getMediaLibrary();
    RepeatMode getRepeatMode() const; // <-- FUNÇÃO ADICIONADA

public slots:
    void playTrack(int index);
    void togglePlayPause();
    void stop();
    void next();
    void prev();
    void setShuffle(bool enabled);
    void setRepeatMode(RepeatMode mode);
    void seek(int position);
    void setVolume(float volume);

signals:
    void trackChanged(const Track& track, int index);
    void playbackStateChanged(sf::SoundSource::Status status);
    void progressUpdated(int currentSeconds, int totalSeconds);
    void volumeChanged(float volume);

private slots:
    void onEngineStatusChange();

private:
    void generateShuffleList();

    AudioEngine m_audioEngine;
    MediaLibrary m_mediaLibrary;
    QTimer* m_progressTimer;

    int m_currentTrackIndex = -1;
    bool m_isShuffle = false;
    RepeatMode m_repeatMode = RepeatMode::None;
    std::vector<int> m_shuffledIndices;
};

#endif //SPOTIFYCLONE_PLAYBACKSERVICE_H