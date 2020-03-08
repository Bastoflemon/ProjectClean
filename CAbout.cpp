#include "CAbout.h"
#include "ui_CAbout.h"
#include <QPixmap>
#include <QImage>
#include "Version.h"
#include <QDesktopServices>
#include <QUrl>

CAbout::CAbout(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CAbout)
{
	ui->setupUi(this);

	QImage Image;
	Image.load(":/Window/Images/Project.png");
	QPixmap pixmap = QPixmap::fromImage(Image);
	int with = ui->labPic->width();
	int height = ui->labPic->height();
	QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
	 //QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	ui->labPic->setPixmap(fitpixmap);
	ui->labVersion->setText(QString::number(CURRENT_VERSION, 'f', 1));
	ui->labInfo->setText(SOFTWARE_INFO);

}

CAbout::~CAbout()
{
	delete ui;
}

void CAbout::on_btnGitHub_clicked()
{
	QDesktopServices::openUrl(QUrl(GITHUB));
}
