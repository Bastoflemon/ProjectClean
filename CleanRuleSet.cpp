#include "CleanRuleSet.h"
#include "ui_CleanRuleSet.h"
#include <QToolButton>
#include <QDebug>
#include <qlistwidget.h>
#include  <QTimer>
#include <QtEvents>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QMessageBox>

#include "OptionEdit.h"
#include "SuffixOrFileNameEdit.h"
CleanRuleSet::CleanRuleSet(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CleanRuleSet)
{
	ui->setupUi(this);
	b_itemAlterFlag_fileName = false;
	b_itemAlterFlag_suffixName = false;
	m_s_previous_optionName = "";
	buttonInit();
	listViewInit();
	dataInit();
	statusBar_init();
	setCentralWidget(ui->splitter_3);
	QTimer::singleShot(500, this, SLOT(init_select_first_row()));
}

CleanRuleSet::~CleanRuleSet()
{
	delete ui;
}


void CleanRuleSet::buttonInit()
{
	// TODO 按钮绑定
	signalMapper = new QSignalMapper(this);
	// 添加项按钮关联
	signalMapper->setMapping(ui->btnAddOption, ButtonID::AddOption);
	connect(ui->btnAddOption, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->btnAddSuffix, ButtonID::AddSuffix);
	connect(ui->btnAddSuffix, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->btnAddFile, ButtonID::AddFile);
	connect(ui->btnAddFile, SIGNAL(clicked()), signalMapper, SLOT(map()));

	// 删除项按钮关联
	signalMapper->setMapping(ui->btnDeleteOption, ButtonID::DeleteOption);
	connect(ui->btnDeleteOption, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->btnDeleteSuffix, ButtonID::DeleteSuffix);
	connect(ui->btnDeleteSuffix, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->btnDeleteFile, ButtonID::DeleteFile);
	connect(ui->btnDeleteFile, SIGNAL(clicked()), signalMapper, SLOT(map()));

	// 关联按钮响应槽函数
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(button_response(int)));

}

void CleanRuleSet::listViewInit()
{
	// TODO listtree 初始化
	// 清理选项
	m_optionListModel = new QStringListModel(this);
	m_listSelection = new QItemSelectionModel(m_optionListModel);
	connect(m_listSelection, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
			this, SLOT(on_currentChanged(QModelIndex, QModelIndex)));
	ui->vSetOption->setModel(m_optionListModel);
	ui->vSetOption->setSelectionModel(m_listSelection);
	ui->vSetOption->setEditTriggers(QAbstractItemView::NoEditTriggers);


	// 后缀名
	m_suffixListModel = new QStringListModel(this);
	ui->vSetSuffix->setModel(m_suffixListModel);
	// 文件名
	m_fileListModel = new QStringListModel(this);
	ui->vSetFile->setModel(m_fileListModel);

}

void CleanRuleSet::dataInit()
{
	// TODO 数据初始化
	QModelIndex index;
	m_cleanOptionInfo = g_cleanOptionInfo;
	for (cleanOptionData data : m_cleanOptionInfo)
	{
		m_optionListModel->insertRow(m_optionListModel->rowCount());
		index = m_optionListModel->index(m_optionListModel->rowCount() - 1, 0);
		m_optionListModel->setData(index, data.name, Qt::DisplayRole);
	}
}

void CleanRuleSet::statusBar_init()
{
	// TODO 状态栏初始化
	current_edit = new QLabel;
	QString str = tr("当前编辑：");
	current_edit->setMinimumWidth(str.size());
	current_edit->setText(str);
	ui->statusbar->addWidget(current_edit);

	current_edit_content = new QLabel;
	current_edit_content->setMinimumWidth(50);
	ui->statusbar->addWidget(current_edit_content);
}

/**
 * \brief
 * \param index 上一项模型索引
 */
void CleanRuleSet::save_previous_content(QModelIndex index)
{
	// TODO 保存上一选中项数据（规则编辑列表）
	QString optionName = m_optionListModel->data(index).toString();
	// 1、匹配数据（根据保存的名称）
	for (int i = 0; i < m_cleanOptionInfo.count(); i++)
	{
		if (m_cleanOptionInfo[i].name == m_s_previous_optionName)
		{
			// 2、找到数据
			if (m_cleanOptionInfo[i].name != optionName)
				m_cleanOptionInfo[i].name = optionName;
			if (b_itemAlterFlag_suffixName)
			{
				QStringList suffixData = m_suffixListModel->stringList();
				QString s_suffix;
				for (QString data : suffixData)
					s_suffix.append(data).append("|");
				m_cleanOptionInfo[i].cleanSuffix = suffixData;
				m_cleanOptionInfo[i].labSuffix = s_suffix.left(s_suffix.size() - 1);
			}
			if (b_itemAlterFlag_fileName)
			{
				QStringList fileData = m_fileListModel->stringList();
				QString s_file;
				for (QString data : fileData)
					s_file.append(data).append("|");
				m_cleanOptionInfo[i].cleanFile = fileData;
				m_cleanOptionInfo[i].labFile = s_file.left(s_file.size() - 1);
			}
		}
	}
}

/**
 * \brief			   打开添加后缀名、文件名界面
 * \param buttonID	   触发按钮的ID
 * \param groupContent 按钮对应的组标题
 * \param list		   按钮对应的list列表
 */
void CleanRuleSet::open_edit_interface(int buttonID, QString groupContent, QStringList list)
{
	// TODO 打开后缀名/文件名添加界面
	SuffixOrFileNameEdit *suffix_or_file_name_edit = new SuffixOrFileNameEdit(buttonID, groupContent, list, this);
	suffix_or_file_name_edit->setAttribute(Qt::WA_DeleteOnClose);
	suffix_or_file_name_edit->setWindowModality(Qt::WindowModal);
	connect(suffix_or_file_name_edit, SIGNAL(addList(int, QStringList)), this, SLOT(addList(int, QStringList)));
	suffix_or_file_name_edit->show();
}

void CleanRuleSet::add_item_file_name(QStringList list)
{
	// TODO 添加文件名list
	QModelIndex index;
	for (QString str : list)
	{
		m_fileListModel->insertRow(m_fileListModel->rowCount());
		index = m_fileListModel->index(m_fileListModel->rowCount() - 1, 0);
		m_fileListModel->setData(index, str, Qt::DisplayRole);
	}
	ui->vSetFile->setCurrentIndex(index);
}

void CleanRuleSet::add_item_suffix_name(QStringList list)
{
	// TODO 添加后缀名list
	QModelIndex index;
	for (QString str : list)
	{
		m_suffixListModel->insertRow(m_suffixListModel->rowCount());
		index = m_suffixListModel->index(m_suffixListModel->rowCount() - 1, 0);
		m_suffixListModel->setData(index, str, Qt::DisplayRole);
	}
	ui->vSetSuffix->setCurrentIndex(index);
}

/**
 * \brief 按钮响应函数
 * \param id button ID
 */
void CleanRuleSet::button_response(int id)
{
	// TODO 按钮响应函数
	QModelIndex index;
	switch (id)
	{
		case ButtonID::AddOption: // 规则名添加界面
		{
			QStringList list = m_optionListModel->stringList();
			OptionEdit *option_edit = new OptionEdit("", -1, list, this);
			option_edit->setAttribute(Qt::WA_DeleteOnClose);
			option_edit->setWindowModality(Qt::WindowModal);
			connect(option_edit, SIGNAL(option_text(QString, int)), this, SLOT(option_text(QString, int)));
			option_edit->show();
			break;
		}
		case ButtonID::AddSuffix: // 后缀名编辑界面
		{
			open_edit_interface(ButtonID::AddSuffix, ui->suffixName->title(), m_suffixListModel->stringList());
			break;
		}
		case ButtonID::AddFile: // 文件名编辑界面
		{
			open_edit_interface(ButtonID::AddFile, ui->fileName->title(), m_fileListModel->stringList());
			break;
		}
		case ButtonID::DeleteOption: // 删除规则名，同时删除vector数据
		{
			index = ui->vSetOption->currentIndex();
			if (index.isValid())
			{
				// 删除对应数据
				for (int i = 0; i < m_cleanOptionInfo.count(); i++)
				{
					if (m_cleanOptionInfo[i].name == m_s_previous_optionName)
					{
						m_cleanOptionInfo.removeAt(i);
						break;
					}
				}
				m_optionListModel->removeRow(index.row());
				QModelIndex index = m_optionListModel->index(m_optionListModel->rowCount() - 1, 0);
				ui->vSetOption->setCurrentIndex(index);
			}
			break;
		}
		case ButtonID::DeleteSuffix: // 删除后缀名
		{
			index = ui->vSetSuffix->currentIndex();
			if (index.isValid())
				m_suffixListModel->removeRow(index.row());
			b_itemAlterFlag_suffixName = true;
			break;
		}
		case ButtonID::DeleteFile: // 删除文件名
		{
			index = ui->vSetFile->currentIndex();
			if (index.isValid())
				m_fileListModel->removeRow(index.row());
			b_itemAlterFlag_fileName = true;
			break;
		}
	}
}

/**
 * \brief      向清理选项列表添加项
 * \param text 添加字符串
 */
void CleanRuleSet::option_text(QString text, int row)
{
	// TODO 清理选项添加界面响应函数
	QModelIndex index;
	if (row == -1)
	{
		// 添加规则名，向vector添加数据
		cleanOptionData clean_option_data{};
		clean_option_data.name = text;
		m_cleanOptionInfo.append(clean_option_data);
		m_optionListModel->insertRow(m_optionListModel->rowCount());
		index = m_optionListModel->index(m_optionListModel->rowCount() - 1, 0);
		m_optionListModel->setData(index, text, Qt::DisplayRole);
		ui->vSetOption->setCurrentIndex(index);
	}
	else
	{
		// 编辑规则名
		index = m_optionListModel->index(row, 0);
		m_optionListModel->setData(index, text, Qt::DisplayRole);
		ui->vSetOption->setCurrentIndex(index);
	}
}

/**
 * \brief		   向对应list列表添加项，并修改标志
 * \param buttonID 按钮ID
 * \param addList  字符串列表
 */
void CleanRuleSet::addList(int buttonID, QStringList addList)
{
	// TODO 后缀/文件名列表添加数据
	QModelIndex index;
	switch (buttonID)
	{
		case ButtonID::AddSuffix:
		{
			add_item_suffix_name(addList);
			b_itemAlterFlag_suffixName = true;
			break;
		}
		case ButtonID::AddFile:
		{
			add_item_file_name(addList);
			b_itemAlterFlag_fileName = true;
			break;
		}
	}
}



void CleanRuleSet::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	// TODO 规则名切换响应函数
	// 保存上一项
	if (previous.isValid())
	{
		save_previous_content(previous);
	}
	// 显示当前项
	if (current.isValid())
	{
		QString current_text = m_optionListModel->data(current, Qt::DisplayRole).toString();
		m_s_previous_optionName = current_text;
		current_edit_content->setText(current_text);

		m_suffixListModel->removeRows(0, m_suffixListModel->rowCount());
		m_fileListModel->removeRows(0, m_fileListModel->rowCount());
		for (cleanOptionData data : m_cleanOptionInfo)
		{
			if (data.name == current_text)
			{
				if (data.cleanSuffix.count() > 0)
					add_item_suffix_name(data.cleanSuffix);
				if (data.cleanFile.count() > 0)
					add_item_file_name(data.cleanFile);
				break;
			}
		}

		b_itemAlterFlag_suffixName = false;
		b_itemAlterFlag_fileName = false;
	}

}

void CleanRuleSet::init_select_first_row()
{
	// TODO 延迟选中规则名第一项
	QModelIndex index = m_optionListModel->index(0, 0);
	ui->vSetOption->setCurrentIndex(index);
}

void CleanRuleSet::save_result(int result)
{
	// TODO 保存结果响应槽函数
	switch (result)
	{
		case FileInfo::FileOpenFail:
		{
			ui->btnSave->setIcon(QIcon(":/Window/Images/err.ico"));
			ui->btnSave->setStyleSheet("color: red");
			break;
		}
		case FileInfo::SUCCEED:
		{
			ui->btnSave->setIcon(QIcon(":/Window/Images/right.ico"));
			ui->btnSave->setStyleSheet("color: green");
			break;
		}
		case FileInfo::No_Save_Content:
			QMessageBox::warning(this, tr("警告"), tr("没有要保存的规则，请重新编辑！"));
			break;
	}
	QTimer::singleShot(5000, this, SLOT(restore_save_button_style()));
}

void CleanRuleSet::restore_save_button_style()
{
	ui->btnSave->setIcon(QIcon(":/Window/Images/Save.png"));
	ui->btnSave->setStyleSheet("color: black");
}

void CleanRuleSet::on_vSetOption_doubleClicked(const QModelIndex &index)
{
	// TODO 规则编辑列表双击响应
	// 打开规则编辑界面
	QString option_name = m_optionListModel->data(index).toString();
	int row = index.row();
	QStringList list = m_optionListModel->stringList();
	list.removeAt(row);
	OptionEdit *option_edit = new OptionEdit(option_name, row, list, this);
	option_edit->setAttribute(Qt::WA_DeleteOnClose);
	option_edit->setWindowModality(Qt::WindowModal);
	connect(option_edit, SIGNAL(option_text(QString, int)), this, SLOT(option_text(QString, int)));
	option_edit->show();
}

void CleanRuleSet::dragEnterEvent(QDragEnterEvent *event)
{
	// TODO 进入拖拽控件时触发
	QString dirPath = event->mimeData()->urls().first().toLocalFile();
	if (!QFileInfo(dirPath).isDir())
		event->acceptProposedAction();
	else
		event->ignore();
}

// 筛选拖拽文件
// 1、带后缀/不带后缀
// 2、过滤已存在的数据
// 3、向对应list视图添加数据
void CleanRuleSet::dropEvent(QDropEvent *event)
{
	// TODO 拖拽状态下释放鼠标触发
	QList<QUrl> urls = event->mimeData()->urls();
	QStringList temp;
	QStringList suffix_list, file_list, view_suffix, view_file;
	view_suffix = m_suffixListModel->stringList();
	view_file = m_fileListModel->stringList();
	for (QUrl url : urls)
	{
		QString path = url.toLocalFile();
		QFileInfo file_info(path);
		if (!file_info.isDir())
		{
			QString fileName = file_info.fileName();
			if (fileName.contains("."))
			{
				fileName = file_info.suffix();
				if (!view_suffix.contains(fileName))
				{
					if (!suffix_list.contains(fileName))
						suffix_list << fileName;
				}
			}
			else
			{
				if (!view_file.contains(fileName))
				{
					if (!file_list.contains(fileName))
						file_list << fileName;
				}
			}
		}
	}
	add_item_suffix_name(suffix_list);
	b_itemAlterFlag_suffixName = true;
	add_item_file_name(file_list);
	b_itemAlterFlag_fileName = true;
}

/*
 * 1、保存上一项数据
 * 2、去除规则后无数据向vector
 * 3、同步全局vector
 * 4、发射保存信号
 */
void CleanRuleSet::on_btnSave_clicked()
{
	// TODO 保存按钮
	QModelIndex index = ui->vSetOption->currentIndex();
	save_previous_content(index);
	for (int i = 0; i < m_cleanOptionInfo.count(); i++)
	{
		if (m_cleanOptionInfo[i].labSuffix.isEmpty() && m_cleanOptionInfo[i].labFile.isEmpty())
		{
			m_cleanOptionInfo.removeAt(i);
		}
	}
	g_cleanOptionInfo = m_cleanOptionInfo;
	emit save();
}
