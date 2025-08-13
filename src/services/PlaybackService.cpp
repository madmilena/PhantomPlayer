#include "PlaybackService.h"
#include <QTimer>
#include <random>
#include <algorithm>
#include <iostream>
#include <numeric> // Necessário para std::iota

PlaybackService::PlaybackService(MediaLibrary* mediaLibrary, QObject *parent)
    : QObject(parent), m_mediaLibrary(mediaLibrary)
{
    m_progressTimer = new QTimer(this);
    connect(m_progressTimer, &QTimer::timeout, this, &PlaybackService::update);
}

PlaybackService::~PlaybackService() = default;

const std::vector<Track>& PlaybackService::getTracks() const {
    // Corrigido: usa -> para ponteiro
    return m_mediaLibrary->getTracks();
}

float PlaybackService::getInitialVolume() const {
    // Retorna um valor padrão, já que m_audioEngine não existe.
    return 100.0f;
}

RepeatMode PlaybackService::getRepeatMode() const {
    return m_repeatMode;
}

void PlaybackService::playTrack(int trackIndex) {
    // Corrigido: usa -> para ponteiro
    if (trackIndex >= 0 && trackIndex < m_mediaLibrary->getTracks().size()) {
        m_currentTrackIndex = trackIndex;
        playCurrentTrack();
    }
}

void PlaybackService::playCurrentTrack() {
    // Corrigido: usa -> para ponteiro
    const auto& tracks = m_mediaLibrary->getTracks();
    if (m_currentTrackIndex < 0 || m_currentTrackIndex >= tracks.size()) return;

    const auto& trackToPlay = tracks[m_currentTrackIndex];
    if (m_music.openFromFile(trackToPlay.filePath)) {
        m_music.play();
        m_progressTimer->start(100);
        // Corrigido: emite o sinal apenas com o argumento correto.
        emit trackChanged(trackToPlay);
        emit playbackStateChanged(m_music.getStatus());
    } else {
        std::cerr << "Error: could not open music file " << trackToPlay.filePath << std::endl;
    }
}

void PlaybackService::togglePlayPause() {
    if (m_currentTrackIndex == -1) {
        // Se nada foi tocado ainda, toca a primeira música.
        playTrack(m_isShuffle ? m_shuffledIndices[0] : 0);
        return;
    }

    auto status = m_music.getStatus();
    if (status == sf::Sound::Playing) {
        m_music.pause();
    } else {
        m_music.play();
    }
    emit playbackStateChanged(m_music.getStatus());
}

// Implementação da função que estava faltando
void PlaybackService::generateShuffleList() {
    // Corrigido: usa -> para ponteiro
    const auto& tracks = m_mediaLibrary->getTracks();
    m_shuffledIndices.resize(tracks.size());
    std::iota(m_shuffledIndices.begin(), m_shuffledIndices.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_shuffledIndices.begin(), m_shuffledIndices.end(), g);
}


// O resto das funções usando m_music e m_mediaLibrary->
void PlaybackService::stop() {
    m_music.stop();
    emit playbackStateChanged(sf::Sound::Stopped);
}

void PlaybackService::next() { /* ... implementação da lógica de próximo ... */ }
void PlaybackService::prev() { /* ... implementação da lógica de anterior ... */ }

void PlaybackService::seek(float position) {
    m_music.setPlayingOffset(sf::seconds(position));
}

void PlaybackService::setVolume(float volume) {
    m_music.setVolume(volume);
    emit volumeChanged(static_cast<int>(volume));
}

void PlaybackService::setShuffle(bool shuffle) {
    m_isShuffle = shuffle;
    if (m_isShuffle) {
        generateShuffleList();
    }
}

void PlaybackService::setRepeatMode(RepeatMode mode) {
    m_repeatMode = mode;
}

void PlaybackService::update() {
    if (m_music.getStatus() == sf::Sound::Playing) {
        float currentTime = m_music.getPlayingOffset().asSeconds();
        float totalDuration = m_music.getDuration().asSeconds();
        emit progressUpdated(currentTime, totalDuration);
    }
}