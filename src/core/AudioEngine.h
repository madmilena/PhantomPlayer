#ifndef SPOTIFYCLONE_AUDIOENGINE_H
#define SPOTIFYCLONE_AUDIOENGINE_H

#include <string>
#include <SFML/Audio.hpp>

class AudioEngine {
public:
    AudioEngine();

    bool play(const std::string& filePath);
    void pause();
    void resume();
    void stop();

    sf::SoundSource::Status getStatus() const;
    sf::Time getPlayingOffset() const;
    void setPlayingOffset(sf::Time offset);

    // --- NOVOS MÉTODOS ---
    void setVolume(float volume); // Volume de 0.0 a 100.0
    float getVolume() const;

private:
    sf::Music m_music;
};

#endif //SPOTIFYCLONE_AUDIOENGINE_H