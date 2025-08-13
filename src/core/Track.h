#ifndef SPOTIFYCLONE_TRACK_H
#define SPOTIFYCLONE_TRACK_H

#include <string>
#include <QImage>

struct Track {
    std::string title;
    std::string artist;
    std::string album;
    std::string filePath;
    int durationInSeconds;
    QImage albumArt;
};

#endif //SPOTIFYCLONE_TRACK_H