#include "PlayerControlsWidget.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

PlayerControlsWidget::PlayerControlsWidget(QWidget *parent) : QWidget(parent), m_repeatMode(RepeatMode::None) {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    auto* controlsLayout = new QHBoxLayout();

    m_progressSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setFixedWidth(150);

    m_shuffleButton = new QPushButton("S", this);
    m_shuffleButton->setCheckable(true);
    m_shuffleButton->setFixedWidth(40);

    m_prevButton = new QPushButton("<<", this);
    m_playPauseButton = new QPushButton("Play", this);
    m_stopButton = new QPushButton("Stop", this);
    m_nextButton = new QPushButton(">>", this);

    m_repeatButton = new QPushButton("R", this);
    m_repeatButton->setFixedWidth(40);

    controlsLayout->addWidget(m_shuffleButton);
    controlsLayout->addWidget(m_prevButton);
    controlsLayout->addWidget(m_playPauseButton);
    controlsLayout->addWidget(m_stopButton);
    controlsLayout->addWidget(m_nextButton);
    controlsLayout->addWidget(m_repeatButton);
    
    auto* volumeLayout = new QHBoxLayout();
    volumeLayout->addStretch();
    volumeLayout->addWidget(new QLabel("Vol:", this));
    volumeLayout->addWidget(m_volumeSlider);

    mainLayout->addWidget(m_progressSlider);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addLayout(volumeLayout);

    connect(m_playPauseButton, &QPushButton::clicked, this, &PlayerControlsWidget::playPauseClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &PlayerControlsWidget::stopClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &PlayerControlsWidget::nextClicked);
    connect(m_prevButton, &QPushButton::clicked, this, &PlayerControlsWidget::prevClicked);
    connect(m_shuffleButton, &QPushButton::toggled, this, &PlayerControlsWidget::shuffleToggled);
    connect(m_repeatButton, &QPushButton::clicked, this, &PlayerControlsWidget::repeatClicked);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayerControlsWidget::volumeChanged);
    connect(m_progressSlider, &QSlider::sliderMoved, this, &PlayerControlsWidget::seeked);
}

void PlayerControlsWidget::onPlaybackStateChanged(const sf::SoundSource::Status status) const
{
    if (status == sf::Music::Status::Playing) {
        m_playPauseButton->setText("Pause");
    } else {
        m_playPauseButton->setText("Play");
    }
}

void PlayerControlsWidget::onProgressUpdated(const int currentSeconds, const int totalSeconds) const
{
    m_progressSlider->blockSignals(true);
    if(m_progressSlider->maximum() != totalSeconds && totalSeconds > 0) {
        m_progressSlider->setRange(0, totalSeconds);
    }
    m_progressSlider->setValue(currentSeconds);
    m_progressSlider->blockSignals(false);
}

void PlayerControlsWidget::onVolumeChanged(const float volume) const
{
    m_volumeSlider->blockSignals(true);
    m_volumeSlider->setValue(static_cast<int>(volume));
    m_volumeSlider->blockSignals(false);
}

void PlayerControlsWidget::setRepeatButtonMode(RepeatMode mode) {
    switch (mode) {
    case RepeatMode::None:
        ui->repeatButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
        break;
        // --- CORREÇÃO AQUI ---
    case RepeatMode::All:
        // Supondo que você tenha um ícone específico, senão use um placeholder
        ui->repeatButton->setIcon(QIcon(":/icons/repeat_all.png")); // Exemplo
        break;
        // --- E CORREÇÃO AQUI ---
    case RepeatMode::One:
        ui->repeatButton->setIcon(QIcon(":/icons/repeat_one.png")); // Exemplo
        break;
    }
}