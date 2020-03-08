#ifndef CABOUT_H
#define CABOUT_H

#include <QDialog>

namespace Ui {
class CAbout;
}

class CAbout : public QDialog
{
    Q_OBJECT

public:
    explicit CAbout(QWidget *parent = nullptr);
    ~CAbout();

private slots:
    void on_btnGitHub_clicked();

private:
    Ui::CAbout *ui;
};

#endif // CABOUT_H
