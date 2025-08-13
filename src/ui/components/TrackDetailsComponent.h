#ifndef PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H
#define PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H

#include <QWidget>
#include <QString>

class QLabel;

class TrackDetailsComponent : public QWidget {
    Q_OBJECT
public:
    explicit TrackDetailsComponent(QWidget* parent = nullptr);

    void setAlbumArt(const QString& imagePath);
    void setTitle(const QString& title);
    void setArtist(const QString& artist);

private:
    QLabel* m_albumArtLabel;
    QLabel* m_titleLabel;
    QLabel* m_artistLabel;

    void setupUI();
};

#endif // PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H
