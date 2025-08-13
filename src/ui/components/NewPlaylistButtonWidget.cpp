#include "NewPlaylistButtonWidget.h"
#include <QHBoxLayout>

NewPlaylistButtonWidget::NewPlaylistButtonWidget(QWidget* parent) : QWidget(parent) {
    m_button = new QPushButton("+ Nova Playlist", this);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_button);

    connect(m_button, &QPushButton::clicked, this, &NewPlaylistButtonWidget::clicked);
}
