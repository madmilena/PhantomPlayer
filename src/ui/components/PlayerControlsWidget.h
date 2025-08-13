#ifndef PHANTOMPLAYER_PLAYERCONTROLSWIDGET_H
#define PHANTOMPLAYER_PLAYERCONTROLSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>

class PlayerControlsWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlayerControlsWidget(QWidget *parent = nullptr);

signals:
    void playPauseClicked();
    void stopClicked();
    void nextClicked();
    void prevClicked();
    void shuffleToggled(bool enabled);
    void repeatClicked();
    void volumeChanged(int value);
    void seeked(int position);

public slots:
    void onPlaybackStateChanged(bool isPlaying);
    void onProgressUpdated(int position);
    void onVolumeChanged(int volume);

private:
    QPushButton* m_playPauseButton;
    QPushButton* m_stopButton;
    QPushButton* m_nextButton;
    QPushButton* m_prevButton;
    QPushButton* m_shuffleButton;
    QPushButton* m_repeatButton;
    QSlider* m_volumeSlider;
    QSlider* m_seekSlider;

    bool m_isPlaying = false;

    void setupUI();
    void setupConnections();
};

#endif //PHANTOMPLAYER_PLAYERCONTROLSWIDGET_H
