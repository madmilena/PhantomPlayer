#ifndef PHANTOMPLAYER_PLAYERCONTROLSCOMPONENT_H
#define PHANTOMPLAYER_PLAYERCONTROLSCOMPONENT_H

#include <QWidget>

class QPushButton;
class QSlider;

class PlayerControlsComponent : public QWidget {
    Q_OBJECT
public:
    explicit PlayerControlsComponent(QWidget* parent = nullptr);

signals:
    void playPauseClicked();
    void stopClicked();
    void nextClicked();
    void prevClicked();
    void shuffleToggled(bool enabled);
    void volumeChanged(int value);
    void seeked(int position);
    void repeatClicked();

public slots:
    void onPlaybackStateChanged(bool isPlaying);
    void onProgressUpdated(int position);
    void onVolumeChanged(int value);

private:
    QPushButton* m_playPauseButton;
    QPushButton* m_stopButton;
    QPushButton* m_nextButton;
    QPushButton* m_prevButton;
    QPushButton* m_repeatButton;
    QPushButton* m_shuffleButton;
    QSlider* m_volumeSlider;
    QSlider* m_seekSlider;

    bool m_isPlaying;

    void setupUI();
    void setupConnections();
};

#endif // PHANTOMPLAYER_PLAYERCONTROLSCOMPONENT_H

