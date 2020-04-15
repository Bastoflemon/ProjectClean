#include "HomePage.h"
#include "ui_HomePage.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include "CleanRuleSet.h"
#include <QFileIconProvider>
#include <QTime>
#include "ErrorMessage.h"
#include <QSettings>
#include <QMessageBox>
#include "Version.h"
#include "CAbout.h"
#include "CUpdate.h"
#include <QDesktopServices>

HomePage::HomePage(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::HomePage)
{
	ui->setupUi(this);
	m_b_begin = false;
	m_b_file_node = false;
	m_b_cleanDirList = false;
	m_b_cleanWaitTree = false;
	m_b_stopDeleteFile = false;
	init();
	QTimer::singleShot(1000, this, SLOT(on_actRefreshComboBox_triggered()));
}

HomePage::~HomePage()
{
	delete ui;
}

void HomePage::init()
{
	// TODO 初始化界面
	// 打开/删除目录按钮绑定act
	ui->btnOpenDir->setDefaultAction(ui->actOpenDir);
	ui->btnDeleteDir->setDefaultAction(ui->actDeleteDir);

	// 编辑按钮绑定
	ui->btnOptionSet->setDefaultAction(ui->actOptionSet);
	ui->btnRefresh->setDefaultAction(ui->actRefreshComboBox);

	// 扫描列表初始化
	m_scanDirListModel = new QStringListModel(this);
	ui->cleanDir->setModel(m_scanDirListModel);
	ui->cleanDir->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->cleanDir->setItemDelegate(&m_itemDelegate);
	connect(&m_itemDelegate, SIGNAL(deleteItem(QModelIndex)), SLOT(deleteItemCleanDir(QModelIndex)));

	// 删除错误列表初始化
	m_errorItemModel = new QStandardItemModel(this);
	ui->errList->setModel(m_errorItemModel);
	ui->errList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// 标签页初始化
	ui->tabWidget->setCurrentIndex(0);

	// 树目录初始化
	ui->cleanWait->setColumnWidth(TreeColNum::colItemName, 210);
	ui->cleanWait->setColumnWidth(TreeColNum::colItemType, 25);

	// 初始化目录
    QString Path = QApplication::applicationDirPath();
    QString imagesPath = Path + "/Images";
    QDir imagesDir(imagesPath);
    if (!imagesDir.exists())
	{
        imagesDir.mkdir(imagesPath);
	}
    QString languagePath = Path + "/Language";
    QDir languageDir(languagePath);
    if (!languageDir.exists())
    {
        languageDir.mkdir(languagePath);
    }
	setCentralWidget(ui->splitterMain);
}

/**
 * \brief 判断清理目录路径是否存在
 * \param itemStr 路径字符串
 * \return
 */
QStringList HomePage::addItemCleanDirList(QStringList &dirPath)
{
	// TODO 获取可添加扫描的目录列表
	QStringList dirList, retList;
	dirList = m_scanDirListModel->stringList();
	if (!dirList.isEmpty())
	{
		for (QString src : dirPath)
		{
			if (!dirList.contains(src))
				retList << src;
		}
	}
	else
		return retList = dirPath;
	return  retList;
}

void HomePage::addItemCleanDir(QString &str)
{
	// TODO 向清理目录添加项
	if (!m_b_cleanDirList)
	{
		m_b_cleanDirList = true;
		ui->actDeleteDir->setEnabled(true);
		ui->btnScan->setEnabled(true);
		ui->btnCleanDir->setEnabled(true);
	}
	m_scanDirListModel->insertRow(m_scanDirListModel->rowCount());
	QModelIndex index = m_scanDirListModel->index(m_scanDirListModel->rowCount() - 1, 0);
	m_scanDirListModel->setData(index, str, Qt::DisplayRole);
}



void HomePage::comboBox_update()
{
	// TODO 下拉列表数据更新
	ui->cleanOption->clear();
	// 下拉列表初始化
	for (cleanOptionData clean : g_cleanOptionInfo)
	{
		QString iconPath = QApplication::applicationDirPath() + "/Images/" + clean.name + ".ico";
		if (QFile(iconPath).exists())
			ui->cleanOption->addItem(QIcon(iconPath), clean.name);
		else
			ui->cleanOption->addItem(clean.name);
	}
}

QIcon HomePage::getFileIcon(QString path)
{
	// TODO 获取系统文件图标
	QFileInfo file_info(path);
	QFileIconProvider icon_provider;
	return icon_provider.icon(file_info);
}

/**
 * \brief 扫描目录，添加符合要求的文件到目录树中
 * \param path 目录路径
 * \param item 目录项节点，用于添加子项
 */
void HomePage::sacnDir(QString path, QTreeWidgetItem *item)
{
	// TODO 扫描目录
	int index = ui->cleanOption->currentIndex();
	QDir dir(path);
	if (!dir.exists()) return;
	dir.setFilter(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	for (QFileInfo info : dir.entryInfoList())
	{
		if (info.fileName() == "." || info.fileName() == "..") continue;
		QString ll = info.fileName();
		if (info.isDir())
		{
			QTreeWidgetItem *fileItem = new QTreeWidgetItem(TreeItemType::itDirItem);  // 目录节点
			fileItem->setIcon(TreeColNum::colItemName, getFileIcon(info.filePath()));
			fileItem->setText(TreeColNum::colItemName, info.fileName());
			fileItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
			item->addChild(fileItem);
			sacnDir(info.filePath(), fileItem);
			if (fileItem->childCount() == 0)
			{
				item->removeChild(fileItem);
				delete fileItem;
			}
		}
		else
		{
			QString fileName = info.fileName();
			if (fileName.contains("."))
			{
				// 后缀
				fileName = info.suffix();
				if (g_cleanOptionInfo[index].cleanSuffix.contains(info.suffix()))
				{
					addFileNode(item, info.filePath(), info.fileName(), info.size(), "." + info.suffix());
				}
			}
			else
			{
				// 文件
				if (g_cleanOptionInfo[index].cleanFile.contains(info.fileName()))
				{
					addFileNode(item, info.filePath(), info.fileName(), info.size(), info.fileName());
				}
			}

		}
	}
}

QString HomePage::fileSizeCalc(int size)
{
	// TODO 字节转换
	QString unit = tr("Byte");
	float fSize = size;
	if (fSize > 1024)
	{
		fSize /= 1024; // KB
		unit = "KB";
	}
	if (fSize > 1024)
	{
		fSize /= 1024; // M
		unit = "M";
	}
	if (fSize > 1024)
	{
		fSize /= 1024; // G
		unit = "G";
	}
	return QString::number(fSize, 'f', 2) + " " + unit;
}



/**
 * \brief 添加文件节点
 * \param item 父项
 * \param filePath 文件路径
 * \param fileName 文件名
 * \param fileSize 文件大小
 * \param fileType 文件类型
 */
void HomePage::addFileNode(QTreeWidgetItem *item, QString filePath, QString fileName, int fileSize, QString fileType)
{
	// TODO 添加文件节点
	QTreeWidgetItem *fileItem = new QTreeWidgetItem(TreeItemType::itFileItem);   // 文件节点
	fileItem->setIcon(TreeColNum::colItemName, getFileIcon(filePath)); // 设置系统图标
	fileItem->setText(TreeColNum::colItemName, fileName);
	fileItem->setText(TreeColNum::colItemSize, fileSizeCalc(fileSize));;
	fileItem->setText(TreeColNum::colItemType, fileType);
	fileItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
	fileItem->setData(TreeColNum::colItemName, Qt::UserRole, QVariant(filePath));
	item->addChild(fileItem);
	if (!m_b_file_node)
		m_b_file_node = true;
}


/*
 * 1、保存文件
 * 2、成功更新下拉列表
 * 3、发射保存结果信号
 */
void HomePage::save()
{
	// TODO 保存信号响应函数
	configFile = new ConfigFile();
	int fileInfoFlag = configFile->save_config_file();
	if (fileInfoFlag == FileInfo::SUCCEED)
	{
		delete configFile;
		comboBox_update();
	}
	emit save_result(fileInfoFlag);
}




void HomePage::dragEnterEvent(QDragEnterEvent *event)
{
	// TODO 当拖拽后进入接收拖拽的控件时触发
	QString dirPath = event->mimeData()->urls().first().toLocalFile();
	if (QFileInfo(dirPath).isDir())
		event->acceptProposedAction();
	else
		event->ignore();
}

void HomePage::dropEvent(QDropEvent *event)
{
	// TODO 拖拽状态下释放鼠标时触发
	// 向清理目录添加项
	QList<QUrl> urls = event->mimeData()->urls();
	QStringList temp;
	for (QUrl url : urls)
	{
		if (QFileInfo(url.toLocalFile()).isDir())
			temp << url.toLocalFile();
	}
	for (QString str : addItemCleanDirList(temp))
		addItemCleanDir(str);
}


void HomePage::deleteItemCleanDir(const QModelIndex &index)
{
	// TODO 清理目录，按钮状态设置
	m_scanDirListModel->removeRow(index.row());

	if (m_scanDirListModel->rowCount() == 0)
	{
		ui->actDeleteDir->setEnabled(false);
		ui->btnScan->setEnabled(false);
		m_b_cleanDirList = false;
	}
	if (m_b_cleanWaitTree)
	{
		ui->cleanWait->clear();
		m_b_cleanWaitTree = false;
		ui->btnDeleteFile->setEnabled(false);
	}
}

void HomePage::on_actOpenDir_triggered()
{
	// TODO 打开目录
	QString curPath = QDir::currentPath();
	// 确认，返回空字符串，默认使用当前目录
	QString selectedDir = QFileDialog::getExistingDirectory(this, tr("选择一个目录"), curPath, QFileDialog::ShowDirsOnly);
	// 选择多目录对话框：返回目录列表 QStringList
	if (!selectedDir.isEmpty() && curPath != selectedDir)
	{
		QStringList temp;
		temp << selectedDir;
		QStringList em = addItemCleanDirList(temp);
		for (QString str : em)
		{
			if (!str.isEmpty())
				addItemCleanDir(str);
		}
	}
}


void HomePage::on_cleanOption_currentIndexChanged(int index)
{
	//TODO 下拉列表选中显示相关信息
	if (index != -1)
	{
		bool suffixShow, fileShow;
		QString tempSuffix = g_cleanOptionInfo[index].labSuffix;
		QString tempFile = g_cleanOptionInfo[index].labFile;
		suffixShow = !tempSuffix.isEmpty();
		fileShow = !tempFile.isEmpty();
		if (suffixShow)
		{
			ui->cleanSuffix->setText(tempSuffix);
			ui->cleanSuffix->setCursorPosition(0);
		}
		if (fileShow)
		{
			ui->cleanFile->setText(tempFile);
			ui->cleanFile->setCursorPosition(0);
		}
		ui->groupSuffix->setVisible(suffixShow);
		ui->groupFile->setVisible(fileShow);
	}
}


void HomePage::on_actOptionSet_triggered()
{
	//TODO 创建配置文件界面
	CleanRuleSet *cleanRuleSet = new CleanRuleSet(this);
	cleanRuleSet->setAttribute(Qt::WA_DeleteOnClose); // 自动删除对话框
	cleanRuleSet->setWindowFlags(cleanRuleSet->windowFlags() | Qt::WindowStaysOnTopHint);

	connect(cleanRuleSet, SIGNAL(save()), this, SLOT(save()));
	connect(this, SIGNAL(save_result(int)), cleanRuleSet, SLOT(save_result(int)));
	cleanRuleSet->setWindowModality(Qt::WindowModal);
	cleanRuleSet->show();

}

void HomePage::on_actDeleteDir_triggered()
{
	// TODO 删除选中清理目录项
	// 更新按钮状态
	QModelIndex index = ui->cleanDir->currentIndex();
	m_scanDirListModel->removeRow(index.row());
	if (m_scanDirListModel->rowCount() == 0)
	{
		ui->actDeleteDir->setEnabled(false);
		ui->btnScan->setEnabled(false);
		m_b_cleanDirList = false;
		ui->btnCleanDir->setEnabled(false);
	}
	if (m_b_cleanWaitTree)
	{
		ui->cleanWait->clear();
		m_b_cleanWaitTree = false;
		ui->btnDeleteFile->setEnabled(false);
	}

}


/*
 * 1、读取配置文件
 * 2、成功，更新下拉列表
 */
void HomePage::on_actRefreshComboBox_triggered()
{
	// TODO 刷新规则列表
	g_cleanOptionInfo = {};
	configFile = new ConfigFile();

	switch (configFile->read_config_file())
	{
		case FileInfo::FileNotExists:
			QMessageBox::warning(this, tr("警告"), tr("未找到配置文件，请重新编辑清理规则！"));
			break;
		case FileInfo::FileOpenFail:
			QMessageBox::warning(this, tr("警告"), tr("文件打开失败"));
			break;
		case FileInfo::SUCCEED:
		{
			delete configFile;
			comboBox_update();
			ui->actOpenDir->setEnabled(true);
			break;
		}
	}
}

/*
 * 1、更新按钮状态
 * 2、扫描目录
 * 2.1、如果无指定扫描文件，则删除该扫描目录
 * 2.2、如果停止扫描，则退出循环
 * 3、更新tree顶层节点状态
 * 4、更新按钮状态
 */
void HomePage::on_btnScan_clicked()
{
	// TODO 扫描按钮
	ui->actOpenDir->setEnabled(false);
	ui->actDeleteDir->setEnabled(false);
	ui->actOptionSet->setEnabled(false);
	ui->actRefreshComboBox->setEnabled(false);

	ui->btnScan->setEnabled(false);
	ui->btnStopScan->setEnabled(true);
	ui->btnDeleteFile->setEnabled(false);
	m_b_file_node = false;

	m_b_begin = true;
	ui->cleanWait->clear();
	QStringList list = m_scanDirListModel->stringList();



	for (QString path : m_scanDirListModel->stringList())
	{
		QFileInfo info(path);
		if (!info.exists())
			continue;
		QTreeWidgetItem *dirItem = new QTreeWidgetItem(TreeItemType::itTopItem);
		dirItem->setIcon(TreeColNum::colItemName, getFileIcon(path));
		dirItem->setText(TreeColNum::colItemName, info.fileName()); // 设置目录名
		dirItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
		ui->cleanWait->addTopLevelItem(dirItem);
		sacnDir(path, dirItem);
		if (!m_b_file_node)
			delete ui->cleanWait->takeTopLevelItem(ui->cleanWait->topLevelItemCount() - 1);//
		if (!m_b_begin)
			break;
	}
	if (ui->cleanWait->topLevelItemCount() > 0)
	{
		m_b_cleanWaitTree = true;
		ui->btnDeleteFile->setEnabled(true);
	}

	ui->actOpenDir->setEnabled(true);
	ui->actDeleteDir->setEnabled(true);
	ui->actOptionSet->setEnabled(true);
	ui->actRefreshComboBox->setEnabled(true);

	ui->btnScan->setEnabled(true);
	ui->btnStopScan->setEnabled(false);

}

void HomePage::on_btnStopScan_clicked()
{
	// TODO 停止扫描按钮
	m_b_begin = false;
	ui->btnStopScan->setEnabled(false);

	ui->btnScan->setEnabled(true);
	ui->btnDeleteFile->setEnabled(true);
	ui->actOpenDir->setEnabled(true);
	ui->actDeleteDir->setEnabled(true);
	ui->actOptionSet->setEnabled(true);
	ui->actRefreshComboBox->setEnabled(true);
}

/*
 *  删除按钮
 *  1、循环获取顶层节点个数，保证循环退出
 *  2、获取第一个节点项
 *  3、调用删除子节点内容函数
 *  4、删除该节点（内存中）
 *  5、如果存在未删除文件，选中错误标签项
 *  6、更新按钮状态
 */
void HomePage::on_btnDeleteFile_clicked()
{
	// TODO 删除文件按钮
	ui->actOpenDir->setEnabled(false);
	ui->actDeleteDir->setEnabled(false);
	ui->actOptionSet->setEnabled(false);
	ui->actRefreshComboBox->setEnabled(false);


	ui->btnScan->setEnabled(false);
	ui->btnDeleteFile->setEnabled(true);
	ui->btnStopDeleteFile->setEnabled(true);

	m_b_stopDeleteFile = false;
	while (!m_b_stopDeleteFile)
	{
		int count = ui->cleanWait->topLevelItemCount();
		if (count == 0)
			break;
		QTreeWidgetItem *item = ui->cleanWait->topLevelItem(0);
		deleteChildItem(item);
		delete ui->cleanWait->takeTopLevelItem(0);
	}
	if (m_errorItemModel->rowCount() > 0)
		ui->tabWidget->setCurrentIndex(1);
	ui->btnStopScan->setEnabled(false);
}


using namespace std;
/**
 * \brief 删除子节点
 * \param superItem 父节点项
 */
void HomePage::deleteChildItem(QTreeWidgetItem *superItem)
{
	// TODO 删除子节点，及文件
	while (true)
	{
		// 1、判断子节点个数，保证退出循环
		if (superItem->childCount() == 0)
			break;

		// 2、获取第1项子节点
		QTreeWidgetItem *childItem = superItem->child(0);
		// 3、判断类型
		// 3.1、目录
		// 3.1.1、调用自身，退出时，删除该项子节点

		// 3.2、文件
		// 3.2.1、删除文件
		// 3.2.2、失败：获取路径、ID、错误文本，添加到错误列表中
		// 3.2.3、删除该节点
		switch (childItem->type())
		{
			case TreeItemType::itDirItem:
			{
				deleteChildItem(childItem);
				superItem->removeChild(childItem);
				delete childItem;
				break;
			}
			case TreeItemType::itFileItem:
			{
				QString path = childItem->data(TreeColNum::colItemName, Qt::UserRole).toString();
				if (!QFile::remove(path))
				{
					DWORD ID = GetLastError();
					string str = to_string(long long(ID));
					QStandardItem *itemPath = new QStandardItem(path);
					itemPath->setData(QString::fromStdString(to_string(long long(ID))), Qt::UserRole);
					itemPath->setData(getLastError(ID), Qt::UserRole + 1);
					m_errorItemModel->appendRow(itemPath);
				}
				superItem->removeChild(childItem);
				delete childItem;
				break;
			}
		}
	}
}
#if _WIN32
/**
 * \brief 获取错误文本
 * \param errorCode 错误ID
 * \return
 */
QString HomePage::getLastError(DWORD errorCode)
{
	// TODO Windows错误信息转文本
	TCHAR *text = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (TCHAR *)&text, 0, NULL);
	LocalFree(text);
	return  QString::fromWCharArray(text);
}
#endif

/**
 * \brief 获取错误信息，并弹窗
 * \param index 选中项
 */
void HomePage::on_errList_doubleClicked(const QModelIndex &index)
{
	// TODO 错误列表双击响应函数
	QString path = m_errorItemModel->data(index).toString();
	QString ID = m_errorItemModel->data(index, Qt::UserRole).toString();
	QString errorStr = m_errorItemModel->data(index, Qt::UserRole + 1).toString();

	ErrorMessage *error_message = new ErrorMessage(path, ID, errorStr, this);
	error_message->setAttribute(Qt::WA_DeleteOnClose);
	error_message->setWindowModality(Qt::WindowModal);
	error_message->show();
}

void HomePage::on_btnStopDeleteFile_clicked()
{
	// TODO 停止删除文件按钮
	m_b_stopDeleteFile = true;
}

void HomePage::on_btnCleanDir_clicked()
{
	// TODO 清理扫描目录
	m_scanDirListModel->removeRows(0, m_scanDirListModel->rowCount());
	ui->btnScan->setEnabled(false);
	ui->actDeleteDir->setEnabled(false);
	ui->btnCleanDir->setEnabled(false);
	m_b_cleanDirList = false;

	if (m_b_cleanWaitTree)
	{
		ui->cleanWait->clear();
		m_b_cleanWaitTree = false;
		ui->btnDeleteFile->setEnabled(false);
	}
}

void HomePage::on_actChinese_triggered()
{
	// TODO 切换中文
	QString path = QApplication::applicationDirPath() + "/Language" + "/ProjectClean.ini";
	QSettings settings(path, QSettings::IniFormat);
	settings.setValue("Language", "zh_CN"); //界面语言，汉语
	QMessageBox::information(this, "提示", "请重启程序应用更改！");
}

void HomePage::on_actEnglish_triggered()
{
	// TODO 切换英文
	QString path = QApplication::applicationDirPath() + "/Language" + "/ProjectClean.ini";
	QSettings settings(path, QSettings::IniFormat);
	settings.setValue("Language", "EN");
	QMessageBox::information(this, "Information", "Please restart the program to apply the changes!");
}

#include <QSslSocket>
void HomePage::on_actCheckUpdate_triggered()
{
	// TODO 检查更新
	QString urlPath = CHECK_UPDATE_URL;
	if (urlPath.isEmpty())
		return;
	QUrl Url = QUrl::fromUserInput(urlPath);
	if (!Url.isValid())
		return;
	m_reply = m_netWorkManager.get(QNetworkRequest(Url));
	connect(m_reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void HomePage::readyRead()
{
	QString str = m_reply->readAll();
	QStringList multi_line = str.split(QRegExp("\n"));
	QString version = multi_line.first().split(QRegExp("# v"), QString::SkipEmptyParts).first();
	float ver = version.toFloat();
	QString updateTime;
	QStringList updateContent;
	if (ver > CURRENT_VERSION)
	{
		multi_line.removeAt(0);
		updateTime = multi_line[0];
		multi_line.removeAt(0);
		for (QString data : multi_line)
		{
			if (data.isEmpty())
				break;
			updateContent << data;
		}
		CUpdate *update = new CUpdate(version, updateTime, updateContent, this);
		update->setAttribute(Qt::WA_DeleteOnClose);
		update->exec();
	}
	else
		QMessageBox::information(this, tr("检查更新"), tr("当前已经是最新版本!"));
	m_reply->deleteLater();
}


void HomePage::on_actProjectClean_triggered()
{
	CAbout *about = new CAbout(this);
	about->setAttribute(Qt::WA_DeleteOnClose);
	about->exec();
}

void HomePage::on_actReadMe_triggered()
{
    QString path = QApplication::applicationDirPath() + "/ReadMe.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
