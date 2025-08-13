#ifndef PHANTOMPLAYER_PLAYLIST_H
#define PHANTOMPLAYER_PLAYLIST_H

#include <string>
#include <vector>

class Playlist {
public:
    explicit Playlist(std::string  name);

    void addTrack(int trackId);
    void removeTrack(int trackIndex);

    // --- CORREÇÃO AQUI: ADICIONANDO MÉTODOS GETTER ---
    [[nodiscard]] const std::string& getName() const { return m_name; }
    [[nodiscard]] const std::vector<int>& getTrackIds() const { return m_trackIds; }

private:
    std::string m_name;
    std::vector<int> m_trackIds;
};

#endif //PHANTOMPLAYER_PLAYLIST_H