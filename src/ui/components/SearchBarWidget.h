#ifndef PHANTOMPLAYER_SEARCHBARWIDGET_H
#define PHANTOMPLAYER_SEARCHBARWIDGET_H

#include <QWidget>

class QLineEdit; // Forward declaration

class SearchBarWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchBarWidget(QWidget* parent = nullptr);
    QString text() const;

signals:
    void searchQueryChanged(const QString& text);

private:
    QLineEdit* m_searchBar;
};

#endif //PHANTOMPLAYER_SEARCHBARWIDGET_H