#ifndef SUFFIXORFILENAMEEDIT_H
#define SUFFIXORFILENAMEEDIT_H

#include <QMainWindow>
#include "CleanRuleSet.h"
namespace Ui {
	class SuffixOrFileNameEdit;
}

/**
 * \brief 后缀名、文件名（无后缀）编辑界面
 */
class SuffixOrFileNameEdit : public QMainWindow
{
	Q_OBJECT

public:
	explicit SuffixOrFileNameEdit(int buttonID, QString content, QStringList strList, QWidget *parent = nullptr);
	~SuffixOrFileNameEdit();
private:
	int m_i_buttonID;
	QStringList m_s_list;
	void removeListSame(QStringList &list);
private slots:
	void on_btnOK_clicked();

signals:
	void addList(int buttonID, QStringList addList);

private:
	Ui::SuffixOrFileNameEdit *ui;
};

#endif // SUFFIXORFILENAMEEDIT_H
