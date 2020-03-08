#ifndef CUPDATE_H
#define CUPDATE_H

#include <QDialog>

namespace Ui {
class CUpdate;
}

class CUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit CUpdate(QString version,QString time,QStringList content,QWidget *parent = nullptr);
    ~CUpdate();

private slots:
    void on_btnDownload_clicked();

private:
    Ui::CUpdate *ui;
};

#endif // CUPDATE_H
