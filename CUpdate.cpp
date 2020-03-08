#include "CUpdate.h"
#include "ui_CUpdate.h"
#include "Version.h"
#include <QDesktopServices>
#include <QUrl>

CUpdate::CUpdate(QString version, QString time, QStringList content, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CUpdate)
{
	ui->setupUi(this);
	ui->labVersion->setText(QString::number(CURRENT_VERSION, 'f', 1) + " => " + version);
	ui->labUpdateTime->setText(time);
	QString updateContent = "";
	for (QString data : content)
	{
		updateContent.append(data).append("\n");
	}
	ui->labUpdateContent->setText(updateContent);
}

CUpdate::~CUpdate()
{
	delete ui;
}

void CUpdate::on_btnDownload_clicked()
{
	QDesktopServices::openUrl(QUrl(GITHUB));
}
