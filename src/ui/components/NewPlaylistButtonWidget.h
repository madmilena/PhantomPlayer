#ifndef PHANTOMPLAYER_NEWPLAYLISTBUTTONWIDGET_H
#define PHANTOMPLAYER_NEWPLAYLISTBUTTONWIDGET_H

#include <QWidget>
#include <QPushButton>

class NewPlaylistButtonWidget : public QWidget {
    Q_OBJECT
public:
    explicit NewPlaylistButtonWidget(QWidget* parent = nullptr);

signals:
    void clicked();

private:
    QPushButton* m_button;
};

#endif // PHANTOMPLAYER_NEWPLAYLISTBUTTONWIDGET_H
