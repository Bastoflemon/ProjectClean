#include "OptionEdit.h"
#include "ui_OptionEdit.h"
#include <QMessageBox>
#include <QTimer>

/**
 * \brief
 * \param optionContent 当前编辑项内容（添加时为空）
 * \param row 当前编辑行（添加时为 -1）
 * \param optionList 清理选项List列表
 * \param parent
 */
OptionEdit::OptionEdit(QString optionContent, int row, QStringList optionList, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OptionEdit),
	m_optionList(optionList),
	m_i_row(row)
{
	ui->setupUi(this);
	b_repeat = false;
	ui->lineEdit->setText(optionContent);
}

OptionEdit::~OptionEdit()
{
	delete ui;
}

/*
 * 1、获取字符串
 * 2、判断重复：true-不重复，绿色图标文字
 *			  false-重复，红色图标文字
 */
void OptionEdit::on_btnCheck_clicked()
{
	// TODO 查重按钮
	QString text = ui->lineEdit->text().trimmed();
	if (!text.isEmpty())
	{
		b_repeat = m_optionList.contains(text);
		if (b_repeat)
		{
			ui->btnCheck->setIcon(QIcon(":/Window/Images/err.ico"));
			ui->btnCheck->setStyleSheet("color: red");
		}
		else
		{
			ui->btnCheck->setIcon(QIcon(":/Window/Images/right.ico"));
			ui->btnCheck->setStyleSheet("color: green");
		}
		QTimer::singleShot(1000, this, SLOT(restore_Check_button_style()));
	}
	else
		QMessageBox::warning(this, tr("警告"), tr("不能为空字符串"));

}


/**
 * \brief 查重，并发射添加/修改内容、行号（添加为 -1）
 */
void OptionEdit::on_btnOK_clicked()
{
	// TODO 确认按钮
	on_btnCheck_clicked();
	if (!b_repeat)
	{
		emit option_text(ui->lineEdit->text().trimmed(), m_i_row);
		QDialog::close();
	}
}

/*
 * 还原检查按钮样式
 */
void OptionEdit::restore_Check_button_style()
{
	ui->btnCheck->setIcon(QIcon(":/Window/Images/query.png"));
	ui->btnCheck->setStyleSheet("color: black");
}
