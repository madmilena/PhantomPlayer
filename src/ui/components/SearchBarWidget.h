#ifndef PHANTOMPLAYER_SEARCHBARWIDGET_H
#define PHANTOMPLAYER_SEARCHBARWIDGET_H

#include <QWidget>
#include <QLineEdit>

class SearchBarWidget : public QWidget {
    Q_OBJECT
public:
    explicit SearchBarWidget(QWidget* parent = nullptr);
    QString text() const;

signals:
    void textChanged(const QString&);

private:
    QLineEdit* m_lineEdit;
};

#endif // PHANTOMPLAYER_SEARCHBARWIDGET_H
