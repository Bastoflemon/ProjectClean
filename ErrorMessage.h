#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QMainWindow>

namespace Ui {
class ErrorMessage;
}

class ErrorMessage : public QMainWindow
{
    Q_OBJECT

public:
    explicit ErrorMessage(QString path,QString id,QString textContent, QWidget *parent = nullptr);
    ~ErrorMessage();

private slots:
    void on_btnDir_clicked();

private:
    QString m_path;

private:
    Ui::ErrorMessage *ui;
};

#endif // ERRORMESSAGE_H
