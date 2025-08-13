#include "SearchBarWidget.h"
#include <QLineEdit>
#include <QVBoxLayout>

SearchBarWidget::SearchBarWidget(QWidget *parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Buscar...");
    layout->addWidget(m_searchBar);
    layout->setContentsMargins(0,0,0,0);

    connect(m_searchBar, &QLineEdit::textChanged, this, &SearchBarWidget::searchQueryChanged);
}

QString SearchBarWidget::text() const {
    return m_searchBar->text();
}