#ifndef PHANTOMPLAYER_PLAYERINFOWIDGET_H
#define PHANTOMPLAYER_PLAYERINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class PlayerInfoWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlayerInfoWidget(QWidget* parent = nullptr);

    void setAlbumArt(const QPixmap& pixmap);
    void setTitle(const QString& title);
    void setArtist(const QString& artist);

private:
    QLabel* m_albumArtLabel;
    QLabel* m_titleLabel;
    QLabel* m_artistLabel;
};

#endif // PHANTOMPLAYER_PLAYERINFOWIDGET_H
