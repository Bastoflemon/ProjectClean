#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <QVector>
#include <QStringList>

/*
 * 清理规则保存的数据
 * 1、规则名
 * 2、后缀名列表
 * 3、后缀名字符串：用于显示
 * 4、文件名（无后缀）列表
 * 5、文件名字符串：用于显示
 */
struct cleanOptionData
{
	QString name;
	QStringList cleanSuffix;
	QString labSuffix;
	QStringList cleanFile;
	QString labFile;
};
// 全局vector：存储配置信息
extern QVector<cleanOptionData> g_cleanOptionInfo;

enum FileInfo
{
	FileNotExists,  // 配置文件不存在
	FileOpenFail,   // 文件打开失败
	SUCCEED,        // 成功
	No_Save_Content // 没有保存内容
};

/*
 * 配置文件各行内容
 * 1、规则名
 * 2、后缀名
 * 3、文件名（无后缀）
 */
enum ConfigFileInfo
{
	RuleName,
	SuffixName,
	FileName
};

/*
 * 配置文件读取、保存
 */
class ConfigFile
{
public:
	ConfigFile();
	int read_config_file();
	int save_config_file();
};

#endif // CONFIGFILE_H
