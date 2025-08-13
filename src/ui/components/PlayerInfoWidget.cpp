#include "PlayerInfoWidget.h"
#include <QVBoxLayout>
#include <QFont>
#include <Qt>

PlayerInfoWidget::PlayerInfoWidget(QWidget* parent) : QWidget(parent) {
    m_albumArtLabel = new QLabel("Nenhuma capa de álbum", this);
    m_albumArtLabel->setAlignment(Qt::AlignCenter);
    m_albumArtLabel->setMinimumSize(250, 250);
    m_titleLabel = new QLabel("Selecione uma música", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    m_titleLabel->setFont(titleFont);
    m_artistLabel = new QLabel("", this);
    m_artistLabel->setAlignment(Qt::AlignCenter);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_albumArtLabel);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_artistLabel);
    layout->addStretch();
}

void PlayerInfoWidget::setAlbumArt(const QPixmap& pixmap) const
{
    if (pixmap.isNull()) {
        m_albumArtLabel->setText("Nenhuma capa de álbum");
        m_albumArtLabel->setPixmap(QPixmap());
    } else {
        m_albumArtLabel->setPixmap(pixmap.scaled(m_albumArtLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void PlayerInfoWidget::setTitle(const QString& title) const
{
    m_titleLabel->setText(title);
}

void PlayerInfoWidget::setArtist(const QString& artist) const
{
    m_artistLabel->setText(artist);
}
