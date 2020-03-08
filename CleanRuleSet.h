#ifndef CLEANRULESET_H
#define CLEANRULESET_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QStringListModel>
#include <QVector>
#include "ConfigFile.h"
#include <QItemSelectionModel>
#include <QLabel>
namespace Ui {
	class CleanRuleSet;
}

class CleanRuleSet : public QMainWindow
{
	Q_OBJECT

public:
	explicit CleanRuleSet(QWidget *parent = nullptr);
	~CleanRuleSet();

private:
	enum ButtonID
	{
		AddOption,
		AddSuffix,
		AddFile,
		DeleteOption,
		DeleteSuffix,
		DeleteFile
	};
	QSignalMapper *signalMapper;
	void buttonInit();
	// 清理选项：列表、模型
	QStringList m_optionList;
	QStringListModel *m_optionListModel;
	QItemSelectionModel *m_listSelection;
	// 后缀名：列表、模型
	QStringList m_suffixList;
	QStringListModel *m_suffixListModel;
	// 文件名：列表、模型
	QStringList m_fileList;
	QStringListModel *m_fileListModel;

	QVector<cleanOptionData> m_cleanOptionInfo;
	void listViewInit();
	void dataInit();
	// 状态栏
	QLabel *current_edit; // 当前编辑
	QLabel *current_edit_content; // 当前编辑文字
	void statusBar_init();
	// 修改/空标志
	bool b_itemAlterFlag_suffixName;
	bool b_itemAlterFlag_fileName;
	// 保存上一项规则名（清理选项切换到当前项时保存）
	QString m_s_previous_optionName;
	void save_previous_content(QModelIndex index);
	// 打开后缀名/文件名添加界面
	void open_edit_interface(int buttonID, QString groupContent, QStringList list);
	// 添加项 
	void add_item_file_name(QStringList list);
	void add_item_suffix_name(QStringList list);
private slots:
	// 按钮响应函数
	void button_response(int id);
	// 清理选项添加界面响应函数
	void option_text(QString text, int row);
	// 添加后缀/文件名列表项
	void addList(int buttonID, QStringList addList);
	void init_select_first_row();
    // 保存结果后续操作
	void save_result(int result);
    // 还原保存按钮样式
    void restore_save_button_style();

	void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);
	void on_vSetOption_doubleClicked(const QModelIndex &index);

	void on_btnSave_clicked();

private:
	Ui::CleanRuleSet *ui;


signals:
	void save();

	// QWidget interface
protected:
	void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
};

#endif // CLEANRULESET_H
