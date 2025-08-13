#include "PlayerControlsWidget.h"
#include <QHBoxLayout>

PlayerControlsWidget::PlayerControlsWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void PlayerControlsWidget::setupUI() {
    m_playPauseButton = new QPushButton("Play", this);
    m_stopButton = new QPushButton("Stop", this);
    m_nextButton = new QPushButton("Next", this);
    m_prevButton = new QPushButton("Prev", this);
    m_shuffleButton = new QPushButton("Shuffle", this);
    m_repeatButton = new QPushButton("Repeat", this);
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_seekSlider = new QSlider(Qt::Horizontal, this);

    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(50);
    m_seekSlider->setRange(0, 100);
    m_seekSlider->setValue(0);

    auto* layout = new QHBoxLayout(this);
    layout->addWidget(m_prevButton);
    layout->addWidget(m_playPauseButton);
    layout->addWidget(m_stopButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_shuffleButton);
    layout->addWidget(m_repeatButton);
    layout->addWidget(m_volumeSlider);
    layout->addWidget(m_seekSlider);
}

void PlayerControlsWidget::setupConnections() {
    connect(m_playPauseButton, &QPushButton::clicked, this, [this]() {
        m_isPlaying = !m_isPlaying;
        m_playPauseButton->setText(m_isPlaying ? "Pause" : "Play");
        emit playPauseClicked();
    });

    connect(m_stopButton, &QPushButton::clicked, this, &PlayerControlsWidget::stopClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &PlayerControlsWidget::nextClicked);
    connect(m_prevButton, &QPushButton::clicked, this, &PlayerControlsWidget::prevClicked);
    connect(m_shuffleButton, &QPushButton::clicked, this, [this]() {
        static bool shuffleOn = false;
        shuffleOn = !shuffleOn;
        m_shuffleButton->setStyleSheet(shuffleOn ? "background-color: lightgreen" : "");
        emit shuffleToggled(shuffleOn);
    });
    connect(m_repeatButton, &QPushButton::clicked, this, &PlayerControlsWidget::repeatClicked);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayerControlsWidget::volumeChanged);
    connect(m_seekSlider, &QSlider::sliderReleased, this, [this]() {
        emit seeked(m_seekSlider->value());
    });
}

void PlayerControlsWidget::onPlaybackStateChanged(bool isPlaying) {
    m_isPlaying = isPlaying;
    m_playPauseButton->setText(isPlaying ? "Pause" : "Play");
}

void PlayerControlsWidget::onProgressUpdated(int position) {
    m_seekSlider->setValue(position);
}

void PlayerControlsWidget::onVolumeChanged(int volume) {
    m_volumeSlider->setValue(volume);
}
