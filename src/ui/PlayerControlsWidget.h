#ifndef SPOTIFYCLONE_PLAYERCONTROLSWIDGET_H
#define SPOTIFYCLONE_PLAYERCONTROLSWIDGET_H // <-- CORRIGIDO AQUI

#include <QWidget>
#include "../services/PlaybackService.h"

// Forward declarations
class QPushButton;
class QSlider;
class QLabel;

class PlayerControlsWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlayerControlsWidget(QWidget *parent = nullptr);

public slots:
    void onPlaybackStateChanged(sf::SoundSource::Status status);
    void onProgressUpdated(int currentSeconds, int totalSeconds);
    void onVolumeChanged(float volume);
    void setRepeatButtonMode(RepeatMode mode);

signals:
    void playPauseClicked();
    void stopClicked();
    void nextClicked();
    void prevClicked();
    void shuffleToggled(bool checked);
    void repeatClicked();
    void volumeChanged(int value);
    void seeked(int position);

private:
    RepeatMode m_repeatMode; // O widget precisa saber o modo atual para o próximo clique
    QPushButton* m_shuffleButton;
    QPushButton* m_prevButton;
    QPushButton* m_playPauseButton;
    QPushButton* m_stopButton;
    QPushButton* m_nextButton;
    QPushButton* m_repeatButton;
    QSlider* m_progressSlider;
    QSlider* m_volumeSlider;
};

#endif //SPOTIFYCLONE_PLAYERCONTROLSWIDGET_H