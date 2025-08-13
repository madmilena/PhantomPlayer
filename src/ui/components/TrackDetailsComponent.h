#ifndef PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H
#define PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H

#include <QWidget>
#include "core/Track.h"

class QLabel; // Forward declaration

class TrackDetailsComponent : public QWidget {
    Q_OBJECT

public:
    explicit TrackDetailsComponent(QWidget* parent = nullptr);

public slots:
    void updateDetails(const Track& track);

private:
    void setupUI();

    QLabel* m_albumArtLabel;
    QLabel* m_titleLabel;
    QLabel* m_artistLabel;
};

#endif //PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H