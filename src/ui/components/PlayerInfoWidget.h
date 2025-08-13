#ifndef PHANTOMPLAYER_PLAYERINFOWIDGET_H
#define PHANTOMPLAYER_PLAYERINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class PlayerInfoWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlayerInfoWidget(QWidget* parent = nullptr);

    void setAlbumArt(const QPixmap& pixmap) const;
    void setTitle(const QString& title) const;
    void setArtist(const QString& artist) const;

private:
    QLabel* m_albumArtLabel;
    QLabel* m_titleLabel;
    QLabel* m_artistLabel;
};

#endif // PHANTOMPLAYER_PLAYERINFOWIDGET_H
