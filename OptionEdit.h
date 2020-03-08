#ifndef OPTIONEDIT_H
#define OPTIONEDIT_H

#include <QDialog>

namespace Ui {
	class OptionEdit;
}

/**
 * \brief 配置选项编辑界面
 */
class OptionEdit : public QDialog
{
	Q_OBJECT

public:
	explicit OptionEdit(QString optionContent, int row, QStringList optionList, QWidget *parent = nullptr);
	~OptionEdit();
private slots:
	void on_btnCheck_clicked();

	void on_btnOK_clicked();

private:
	QStringList m_optionList;
	bool b_repeat;
	int m_i_row;
	void restore_Check_button_style();
signals:
	void option_text(QString text, int row);
private:
	Ui::OptionEdit *ui;
};

#endif // OPTIONEDIT_H
