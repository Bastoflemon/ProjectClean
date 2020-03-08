#include "ConfigFile.h"
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QRegExp>
ConfigFile::ConfigFile()
{

}
QVector<cleanOptionData> g_cleanOptionInfo{};

/*
 * 1、加载配置文件
 * 2、逐行读取信息并保存到全局vector
 */
int ConfigFile::read_config_file()
{
	// TODO 读取配置文件
	QString path = QApplication::applicationDirPath() + "/Config.ini";
	QFile file(path);
	if (!file.exists())
		return  FileInfo::FileNotExists;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return FileInfo::FileOpenFail;
	QTextStream textStream(&file); //用文本流读取文件
	textStream.setAutoDetectUnicode(true);

	cleanOptionData clean_option_data = {};

	int fileInfo = ConfigFileInfo::RuleName;

	while (!textStream.atEnd())
	{
		QString line = textStream.readLine().trimmed();//读取文件的一行
		if (line == "-- end --")
			break;
		switch (fileInfo)
		{
			case ConfigFileInfo::RuleName:
			{
				clean_option_data.name = line.split(QRegExp("# "), QString::SkipEmptyParts).first();
				fileInfo = ConfigFileInfo::SuffixName;
				break;
			}
			case ConfigFileInfo::SuffixName:
			{
				clean_option_data.labSuffix = line;
				clean_option_data.cleanSuffix = line.split(QRegExp("\\|"), QString::SkipEmptyParts);
				fileInfo = ConfigFileInfo::FileName;
				break;
			}
			case ConfigFileInfo::FileName:
			{
				clean_option_data.labFile = line;
				clean_option_data.cleanFile = line.split(QRegExp("\\|"), QString::SkipEmptyParts);
				g_cleanOptionInfo.append(clean_option_data);
				clean_option_data = {};
				fileInfo = ConfigFileInfo::RuleName;
				break;
			}
		}
	}
	return FileInfo::SUCCEED;
}

/*
 * 读取vector，保存信息
 */
int ConfigFile::save_config_file()
{
	// TODO 保存配置，写入文件
	if (g_cleanOptionInfo.size() == 0)
		return FileInfo::No_Save_Content;
	QString path = QApplication::applicationDirPath() + "/Config.ini";
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return FileInfo::FileOpenFail;
	QTextStream textStream(&file); //用文本流读取文件
	textStream.setAutoDetectUnicode(true);
	for (cleanOptionData data : g_cleanOptionInfo)
	{
		textStream << "# " << data.name << "\n";
		textStream << data.labSuffix << "\n";
		textStream << data.labFile << "\n";
	}
	textStream << "-- end --";
	file.close();
	return FileInfo::SUCCEED;
}
