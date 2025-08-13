#include "TrackDetailsComponent.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>

TrackDetailsComponent::TrackDetailsComponent(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void TrackDetailsComponent::setupUI() {
    auto* layout = new QVBoxLayout(this);
    m_albumArtLabel = new QLabel("Nenhuma música tocando", this);
    m_albumArtLabel->setAlignment(Qt::AlignCenter);
    m_albumArtLabel->setMinimumSize(250, 250);
    m_albumArtLabel->setStyleSheet("background-color: #282828; border-radius: 5px;");

    m_titleLabel = new QLabel("Selecione uma música", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    m_titleLabel->setFont(titleFont);

    m_artistLabel = new QLabel("", this);
    m_artistLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(m_albumArtLabel);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_artistLabel);
}

void TrackDetailsComponent::updateDetails(const Track& track) const
{
    m_titleLabel->setText(QString::fromStdString(track.title));
    m_artistLabel->setText(QString::fromStdString(track.artist));

    if (!track.albumArt.isNull()) {
        const QPixmap pixmap = QPixmap::fromImage(track.albumArt);
        m_albumArtLabel->setPixmap(pixmap.scaled(m_albumArtLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        m_albumArtLabel->clear();
        m_albumArtLabel->setText("Nenhuma capa de álbum");
    }
}
