#include "SearchBarWidget.h"
#include <QHBoxLayout>

SearchBarWidget::SearchBarWidget(QWidget* parent) : QWidget(parent) {
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText("Buscar na biblioteca...");

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_lineEdit);

    connect(m_lineEdit, &QLineEdit::textChanged, this, &SearchBarWidget::textChanged);
}

QString SearchBarWidget::text() const {
    return m_lineEdit->text();
}
