#include "Playlist.h"
#include <algorithm>

// O construtor agora inicializa o membro m_name corretamente.
Playlist::Playlist(std::string name) : m_name(std::move(name)) {}

void Playlist::addTrack(const int trackId) {
    // Evita adicionar faixas duplicadas na mesma playlist
    if (std::find(m_trackIds.begin(), m_trackIds.end(), trackId) == m_trackIds.end()) {
        m_trackIds.push_back(trackId);
    }
}

void Playlist::removeTrack(const int trackIndex) {
    if (trackIndex >= 0 && trackIndex < m_trackIds.size()) {
        m_trackIds.erase(m_trackIds.begin() + trackIndex);
    }
}