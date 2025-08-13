#ifndef SPOTIFYCLONE_PLAYLIST_H
#define SPOTIFYCLONE_PLAYLIST_H

#include <QString>
#include <vector>

struct Playlist {
    QString name;
    std::vector<int> trackIndices;
};

#endif //SPOTIFYCLONE_PLAYLIST_H