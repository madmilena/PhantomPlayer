#ifndef PHANTOMPLAYER_SEARCHBARWIDGET_H
#define PHANTOMPLAYER_SEARCHBARWIDGET_H

#include <QWidget>

class QLineEdit;

class SearchBarWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchBarWidget(QWidget* parent = nullptr);

signals:
    void searchQueryChanged(const QString& text); // <-- SINAL ADICIONADO

private:
    QLineEdit* m_searchBar;
};

#endif //PHANTOMPLAYER_SEARCHBARWIDGET_H