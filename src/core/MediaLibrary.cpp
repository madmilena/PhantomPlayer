#include "MediaLibrary.h"
#include <iostream>
#include <filesystem>
#include <QImage>

// Includes da TagLib para extração de imagem
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/flacfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/vorbisfile.h>

namespace fs = std::filesystem;

// Função auxiliar para extrair a capa de diferentes tipos de arquivo
QImage imageFromTagLib(const TagLib::FileRef& fileRef) {
    // Tenta para MP3 (ID3v2)
    auto mpegFile = dynamic_cast<TagLib::MPEG::File*>(fileRef.file());
    if (mpegFile != nullptr && mpegFile->ID3v2Tag()) {
        TagLib::ID3v2::Tag* tag = mpegFile->ID3v2Tag();
        TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];
        if (!frameList.isEmpty()) {
            auto* pictureFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());
            if (pictureFrame) {
                QImage image;
                image.loadFromData(reinterpret_cast<const uchar*>(pictureFrame->picture().data()), pictureFrame->picture().size());
                return image;
            }
        }
    }

    // Tenta para FLAC/OGG (Vorbis comments)
    auto xiphComment = dynamic_cast<TagLib::Ogg::XiphComment*>(fileRef.tag());
    if (xiphComment) {
        const TagLib::Ogg::FieldListMap& fieldListMap = xiphComment->fieldListMap();
        if (fieldListMap.contains("METADATA_BLOCK_PICTURE")) {
            const TagLib::StringList& stringList = fieldListMap["METADATA_BLOCK_PICTURE"];
            if (!stringList.isEmpty()) {
                TagLib::String str = stringList.front();
                std::string str8 = str.to8Bit(true);
                TagLib::ByteVector encodedData = TagLib::ByteVector::fromCString(str8.c_str());
                TagLib::ByteVector decodedData = TagLib::ByteVector::fromBase64(encodedData);
                TagLib::FLAC::Picture picture;
                picture.parse(decodedData);
                QImage image;
                image.loadFromData(reinterpret_cast<const uchar*>(picture.data().data()), picture.data().size());
                return image;
            }
        }
    }
    return {}; // Retorna uma imagem nula se não encontrar nada
}

void MediaLibrary::scanDirectory(const std::string& path) {
    m_tracks.clear();
    std::cout << "Escaneando diretorio: " << path << "..." << std::endl;

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            std::string extension = entry.path().extension().string();

            if (extension == ".mp3" || extension == ".flac" || extension == ".ogg" || extension == ".wav") {
                TagLib::FileRef f(filePath.c_str());

                if (!f.isNull() && f.tag()) {
                    TagLib::Tag *tag = f.tag();
                    
                    Track track;
                    track.filePath = filePath;
                    track.title = tag->title().toCString(true);
                    track.artist = tag->artist().toCString(true);
                    track.album = tag->album().toCString(true);

                    if (track.title.empty()) {
                        track.title = entry.path().stem().string();
                    }

                    if (f.audioProperties()) {
                        track.durationInSeconds = f.audioProperties()->lengthInSeconds();
                    }
                    
                    // Extrai a capa do álbum
                    track.albumArt = imageFromTagLib(f);

                    m_tracks.push_back(track);
                }
            }
        }
    }
    std::cout << m_tracks.size() << " musicas encontradas." << std::endl;
}

const std::vector<Track>& MediaLibrary::getTracks() const {
    return m_tracks;
}