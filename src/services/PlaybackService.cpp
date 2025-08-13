#include "PlaybackService.h"
#include <QTimer>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>

PlaybackService::PlaybackService(QObject *parent) : QObject(parent) {
    m_mediaLibrary.scanDirectory("/Users/milenamadsen/Music");
    generateShuffleList();

    m_progressTimer = new QTimer(this);
    m_progressTimer->setInterval(500);
    connect(m_progressTimer, &QTimer::timeout, this, &PlaybackService::onEngineStatusChange);
}

const std::vector<Track>& PlaybackService::getTracks() const {
    return m_mediaLibrary.getTracks();
}

float PlaybackService::getInitialVolume() const {
    return m_audioEngine.getVolume();
}

MediaLibrary* PlaybackService::getMediaLibrary() {
    return &m_mediaLibrary;
}

// --- IMPLEMENTAÇÃO DA FUNÇÃO QUE FALTAVA ---
RepeatMode PlaybackService::getRepeatMode() const {
    return m_repeatMode;
}
// ------------------------------------------

void PlaybackService::generateShuffleList() {
    const auto& tracks = m_mediaLibrary.getTracks();
    m_shuffledIndices.resize(tracks.size());
    std::iota(m_shuffledIndices.begin(), m_shuffledIndices.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_shuffledIndices.begin(), m_shuffledIndices.end(), g);
}

void PlaybackService::playTrack(int index) {
    const auto& tracks = getTracks();
    if (index < 0 || index >= tracks.size()) return;

    m_currentTrackIndex = index;
    const Track& trackToPlay = tracks[m_currentTrackIndex];

    m_audioEngine.play(trackToPlay.filePath);
    m_progressTimer->start();

    emit trackChanged(trackToPlay, m_currentTrackIndex);
    emit playbackStateChanged(m_audioEngine.getStatus());
}

void PlaybackService::togglePlayPause() {
    if (m_currentTrackIndex == -1 && !getTracks().empty()) {
        playTrack(m_isShuffle ? m_shuffledIndices[0] : 0);
        return;
    }

    auto status = m_audioEngine.getStatus();
    if (status == sf::Music::Status::Playing) {
        m_audioEngine.pause();
    } else {
        m_audioEngine.resume();
    }
    emit playbackStateChanged(m_audioEngine.getStatus());
}

void PlaybackService::stop() {
    m_audioEngine.stop();
    m_currentTrackIndex = -1;
    m_progressTimer->stop();
    emit playbackStateChanged(sf::Music::Status::Stopped);
    emit progressUpdated(0, 0);
}

void PlaybackService::next() {
    const auto& tracks = getTracks();
    if (tracks.empty()) return;

    if (m_repeatMode == RepeatMode::RepeatOne && m_currentTrackIndex != -1) {
        playTrack(m_currentTrackIndex);
        return;
    }

    int nextIndex = -1;
    if (m_isShuffle) {
        auto it = std::find(m_shuffledIndices.begin(), m_shuffledIndices.end(), m_currentTrackIndex);
        int shufflePos = (it == m_shuffledIndices.end()) ? -1 : std::distance(m_shuffledIndices.begin(), it);
        
        int nextShufflePos = shufflePos + 1;
        if (nextShufflePos < m_shuffledIndices.size()) {
            nextIndex = m_shuffledIndices[nextShufflePos];
        }
    } else {
        if (m_currentTrackIndex + 1 < tracks.size()) {
            nextIndex = m_currentTrackIndex + 1;
        }
    }

    if (nextIndex != -1) {
        playTrack(nextIndex);
    } else if (m_repeatMode == RepeatMode::RepeatAll) {
        playTrack(m_isShuffle ? m_shuffledIndices[0] : 0);
    } else {
        stop();
    }
}

void PlaybackService::prev() {
    const auto& tracks = getTracks();
    if (tracks.empty()) return;

    if (m_repeatMode == RepeatMode::RepeatOne && m_currentTrackIndex != -1) {
        playTrack(m_currentTrackIndex);
        return;
    }

    int prevIndex = -1;
    if (m_isShuffle) {
        auto it = std::find(m_shuffledIndices.begin(), m_shuffledIndices.end(), m_currentTrackIndex);
        int shufflePos = (it == m_shuffledIndices.end()) ? -1 : std::distance(m_shuffledIndices.begin(), it);

        int prevShufflePos = shufflePos - 1;
        if (prevShufflePos >= 0) {
            prevIndex = m_shuffledIndices[prevShufflePos];
        } else if (m_repeatMode == RepeatMode::RepeatAll) {
            prevIndex = m_shuffledIndices.back();
        }
    } else {
        int tempIndex = m_currentTrackIndex - 1;
        if (tempIndex >= 0) {
            prevIndex = tempIndex;
        } else if (m_repeatMode == RepeatMode::RepeatAll) {
            prevIndex = tracks.size() - 1;
        }
    }

    if (prevIndex != -1) {
        playTrack(prevIndex);
    } else {
        stop();
    }
}

void PlaybackService::setShuffle(bool enabled) {
    if (m_isShuffle != enabled) {
        m_isShuffle = enabled;
        if (m_isShuffle) {
            generateShuffleList();
        }
    }
}

void PlaybackService::setRepeatMode(RepeatMode mode) {
    m_repeatMode = mode;
}

void PlaybackService::seek(int position) {
    m_audioEngine.setPlayingOffset(sf::seconds(static_cast<float>(position)));
}

void PlaybackService::setVolume(float volume) {
    m_audioEngine.setVolume(volume);
    emit volumeChanged(volume);
}

void PlaybackService::onEngineStatusChange() {
    auto status = m_audioEngine.getStatus();
    if (status == sf::Music::Status::Playing) {
        if (m_currentTrackIndex != -1) {
            int current = m_audioEngine.getPlayingOffset().asSeconds();
            int total = getTracks()[m_currentTrackIndex].durationInSeconds;
            emit progressUpdated(current, total);
        }
    } else if (status == sf::Music::Status::Stopped && m_currentTrackIndex != -1) {
        next();
    }
}