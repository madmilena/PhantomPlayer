#include "TrackDetailsComponent.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QSizePolicy>

TrackDetailsComponent::TrackDetailsComponent(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void TrackDetailsComponent::setupUI() {
    m_albumArtLabel = new QLabel(this);
    m_albumArtLabel->setAlignment(Qt::AlignCenter);
    m_albumArtLabel->setMinimumSize(250, 250);
    m_albumArtLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_titleLabel = new QLabel("Selecione uma música", this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_artistLabel = new QLabel("", this);
    m_artistLabel->setAlignment(Qt::AlignCenter);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_albumArtLabel);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_artistLabel);
    layout->addStretch();
}

void TrackDetailsComponent::setAlbumArt(const QString& imagePath) {
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        m_albumArtLabel->setText("Nenhuma música tocando");
    } else {
        m_albumArtLabel->setPixmap(pixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void TrackDetailsComponent::setTitle(const QString& title) {
    m_titleLabel->setText(title);
}

void TrackDetailsComponent::setArtist(const QString& artist) {
    m_artistLabel->setText(artist);
}
