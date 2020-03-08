#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QStringListModel>
#include <QDir>
#include <QFileDialog>
#include <QtEvents>
#include <QDragEnterEvent>
#include <QMimeData>
#include  <QString>
#include "ItemDelegate.h"
#include "ConfigFile.h"
//网络相关头文件
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <Windows.h>
#include <QStandardItemModel>
class QTreeWidgetItem;
QT_BEGIN_NAMESPACE
namespace Ui { class HomePage; }
QT_END_NAMESPACE

enum TreeColNum
{
	colItemName,
	colItemSize,
	colItemType
};

enum TreeItemType
{
	itTopItem = 1001,
	itFileItem,
	itDirItem
};

class HomePage : public QMainWindow
{
	Q_OBJECT

public:
	HomePage(QWidget *parent = nullptr);
	~HomePage();
	ItemDelegate m_itemDelegate;
public slots:
	void deleteItemCleanDir(const QModelIndex &index);
private slots:
	void on_actOpenDir_triggered();

	//void on_actDeleteItem_triggered();

	void on_cleanOption_currentIndexChanged(int index); // 下拉列表响应


	void on_actOptionSet_triggered();

	void on_actDeleteDir_triggered();

	void on_actRefreshComboBox_triggered();

	void on_btnScan_clicked();

	void on_btnStopScan_clicked();

	void on_btnDeleteFile_clicked();

	void on_errList_doubleClicked(const QModelIndex &index);

	void on_btnStopDeleteFile_clicked();

	void on_btnCleanDir_clicked();

	void on_actChinese_triggered();

	void on_actEnglish_triggered();

	void on_actCheckUpdate_triggered();

	void on_actProjectClean_triggered();

    void on_actReadMe_triggered();

private:
	void init();

	QStringList addItemCleanDirList(QStringList &dirPath); // 获取可添加list列表
	void addItemCleanDir(QString &str); // 向清理列表添加项
	void comboBox_update();
	QIcon getFileIcon(QString path); // 获取本地图标
	void sacnDir(QString path, QTreeWidgetItem *item); // 扫描目录
	QString fileSizeCalc(int size); // 文件大小转换
	void deleteChildItem(QTreeWidgetItem *superItem);

	void addFileNode(QTreeWidgetItem *item, QString filePath, QString fileName, int fileSize, QString fileType);
	QString getLastError(DWORD errorCode);

	QNetworkAccessManager m_netWorkManager;     // 网络管理
	QNetworkReply *m_reply; // 网络响应
public slots:
	void save();
	void readyRead(); // 读取下载缓冲区临时数据
private:
	QStringList m_scanDirList; // 待扫描目录列表
	QStringListModel *m_scanDirListModel; // 待扫描目录模型
	QStandardItemModel *m_errorItemModel; // 删除错误列表模型
	ConfigFile *configFile;
	bool m_b_begin;
	bool m_b_file_node;
	bool m_b_cleanDirList;
	bool m_b_cleanWaitTree;
	bool m_b_stopDeleteFile;
signals:
	void save_result(int result);
private:
	Ui::HomePage *ui;

	// QWidget interface
protected:
	void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
};
#endif // HOMEPAGE_H
