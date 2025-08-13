#ifndef PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H
#define PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H

#include <QWidget>
#include "core/Track.h"

class QLabel;

class TrackDetailsComponent : public QWidget {
    Q_OBJECT

public:
    explicit TrackDetailsComponent(QWidget* parent = nullptr);

public slots:
    void updateDetails(const Track& track); // <-- NOME CORRIGIDO

private:
    QLabel* m_albumArtLabel;
    QLabel* m_titleLabel;
    QLabel* m_artistLabel;
};

#endif //PHANTOMPLAYER_TRACKDETAILSCOMPONENT_H