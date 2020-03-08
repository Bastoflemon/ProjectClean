#include "ErrorMessage.h"
#include "ui_ErrorMessage.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>

/**
 * \brief
 * \param path 文件路径
 * \param id 错误ID号
 * \param textContent 错误描述
 * \param parent
 */
ErrorMessage::ErrorMessage(QString path, QString id, QString textContent, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ErrorMessage),
	m_path(path)
{
	ui->setupUi(this);
	ui->labID->setText(id);
	ui->labContent->setText(textContent);
}

ErrorMessage::~ErrorMessage()
{
	delete ui;
}

void ErrorMessage::on_btnDir_clicked()
{
	// TODO 打开文件目录
	QString dir = QFileInfo(m_path).path();
	QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
}
