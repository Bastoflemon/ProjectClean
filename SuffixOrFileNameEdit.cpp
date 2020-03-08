#include "SuffixOrFileNameEdit.h"
#include "ui_SuffixOrFileNameEdit.h"
#include <QTextDocument>
#include <QTextBlock>
#include <QPalette>

/**
 * \brief
 * \param buttonID 按钮ID（触发的按钮）
 * \param content 当前编辑内容： 按钮ID所对应的组内容（后缀/文件）
 * \param strList 已添加的数据列表
 * \param parent
 */
SuffixOrFileNameEdit::SuffixOrFileNameEdit(int buttonID, QString content, QStringList strList, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SuffixOrFileNameEdit),
	m_i_buttonID(buttonID),
	m_s_list(strList)
{
	ui->setupUi(this);
	setCentralWidget(ui->splitter);
	ui->labContent->setText(content);
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::blue);
	ui->labContent->setPalette(pa);
}

SuffixOrFileNameEdit::~SuffixOrFileNameEdit()
{
	delete ui;
}

/*
 * list列表自身去重
 */
void SuffixOrFileNameEdit::removeListSame(QStringList &list)
{
	for (int i = 0; i < list.count(); i++)
	{
		for (int j = i + 1; j < list.count(); j++)
		{
			if (list.at(i) == list.at(j))
			{
				list.removeAt(j);
				j--;
			}
		}
	}
}

/*
 * 1、获取每行文本内容
 * 2、去除已经添加的内容，并组成list
 * 3、对list自身去重
 * 4、发射信号：按钮ID，添加list
 */
void SuffixOrFileNameEdit::on_btnOK_clicked()
{
	// TODO 确认按钮
	QTextDocument *doc = ui->plainTextEdit->document();
	int count = doc->blockCount();
	QStringList textList;
	for (int i = 0; i < count; i++)
	{
		QTextBlock textLine = doc->findBlockByLineNumber(i);
		QString text = textLine.text().trimmed();
		if (!text.isEmpty())
		{
			if (!m_s_list.contains(text))
				textList << text;
		}
	}
	removeListSame(textList);
	emit addList(m_i_buttonID, textList);
	QMainWindow::close();
}
