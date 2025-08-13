#include "PlayerControlsComponent.h"
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>

PlayerControlsComponent::PlayerControlsComponent(QWidget* parent) : QWidget(parent), m_isPlaying(false) {
    setupUI();
    setupConnections();
}

void PlayerControlsComponent::setupUI() {
    m_playPauseButton = new QPushButton("Play", this);
    m_stopButton = new QPushButton("Stop", this);
    m_nextButton = new QPushButton("Next", this);
    m_prevButton = new QPushButton("Prev", this);
    m_repeatButton = new QPushButton("Repeat", this);
    m_shuffleButton = new QPushButton("Shuffle", this);
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_seekSlider = new QSlider(Qt::Horizontal, this);

    auto* layout = new QHBoxLayout(this);
    layout->addWidget(m_prevButton);
    layout->addWidget(m_playPauseButton);
    layout->addWidget(m_stopButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_repeatButton);
    layout->addWidget(m_shuffleButton);
    layout->addWidget(m_seekSlider);
    layout->addWidget(m_volumeSlider);
}

void PlayerControlsComponent::setupConnections() {
    connect(m_playPauseButton, &QPushButton::clicked, this, &PlayerControlsComponent::playPauseClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &PlayerControlsComponent::stopClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &PlayerControlsComponent::nextClicked);
    connect(m_prevButton, &QPushButton::clicked, this, &PlayerControlsComponent::prevClicked);
    connect(m_repeatButton, &QPushButton::clicked, this, &PlayerControlsComponent::repeatClicked);
    connect(m_shuffleButton, &QPushButton::toggled, this, &PlayerControlsComponent::shuffleToggled);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayerControlsComponent::volumeChanged);
    connect(m_seekSlider, &QSlider::sliderMoved, this, &PlayerControlsComponent::seeked);
}

void PlayerControlsComponent::onPlaybackStateChanged(bool isPlaying) {
    m_isPlaying = isPlaying;
    m_playPauseButton->setText(isPlaying ? "Pause" : "Play");
}

void PlayerControlsComponent::onProgressUpdated(const int position) const
{
    m_seekSlider->setValue(position);
}

void PlayerControlsComponent::onVolumeChanged(const int value) const
{
    m_volumeSlider->setValue(value);
}

