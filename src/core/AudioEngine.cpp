#include "AudioEngine.h"
#include <iostream>

AudioEngine::AudioEngine() {
    m_music.setVolume(50); // Vamos definir um volume padrão de 50%
}

bool AudioEngine::play(const std::string& filePath) {
    if (!m_music.openFromFile(filePath)) {
        std::cerr << "AudioEngine Error: Nao foi possivel carregar a musica de: " << filePath << std::endl;
        return false;
    }
    m_music.play();
    std::cout << "AudioEngine: Tocando " << filePath << std::endl;
    return true;
}

void AudioEngine::pause() {
    m_music.pause();
}

void AudioEngine::resume() {
    m_music.play();
}

void AudioEngine::stop() {
    m_music.stop();
}

sf::SoundSource::Status AudioEngine::getStatus() const {
    return m_music.getStatus();
}

sf::Time AudioEngine::getPlayingOffset() const {
    return m_music.getPlayingOffset();
}

void AudioEngine::setPlayingOffset(sf::Time offset) {
    m_music.setPlayingOffset(offset);
}

// --- NOVAS IMPLEMENTAÇÕES ---
void AudioEngine::setVolume(float volume) {
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    m_music.setVolume(volume);
}

float AudioEngine::getVolume() const {
    return m_music.getVolume();
}