#ifndef SPOTIFYCLONE_MEDIALIBRARY_H
#define SPOTIFYCLONE_MEDIALIBRARY_H

#include <vector>
#include <string>
#include "Track.h"

class MediaLibrary {
public:
    // Varre um diretório em busca de arquivos de música
    void scanDirectory(const std::string& path);

    // Retorna uma referência constante para a lista de faixas
    const std::vector<Track>& getTracks() const;

private:
    std::vector<Track> m_tracks;
};

#endif //SPOTIFYCLONE_MEDIALIBRARY_H