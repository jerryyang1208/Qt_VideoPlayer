#include <QApplication>
#include <QLoggingCategory>
#include "VideoPlayer.h"

#include <windows.h>

// 自定义消息处理器 —— 输出时临时切换控制台代码页为 UTF-8，输出完后恢复
static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    const UINT oldCP = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);

    const QByteArray utf8 = msg.toUtf8();
    switch (type) {
    case QtDebugMsg:    fprintf(stderr, "%s\n",           utf8.constData()); break;
    case QtInfoMsg:     fprintf(stderr, "Info: %s\n",     utf8.constData()); break;
    case QtWarningMsg:  fprintf(stderr, "Warning: %s\n",  utf8.constData()); break;
    case QtCriticalMsg: fprintf(stderr, "Critical: %s\n", utf8.constData()); break;
    case QtFatalMsg:    fprintf(stderr, "Fatal: %s\n",    utf8.constData()); abort();
    }

    SetConsoleOutputCP(oldCP);
    OutputDebugStringW(msg.toStdWString().c_str());  // 同步输出到调试器
}

int main(int argc, char *argv[])
{
    // 1. 安装消息处理器（需在 QApplication 之前）
    qInstallMessageHandler(messageHandler);
    SetConsoleOutputCP(CP_UTF8);

    // 2. 过滤 Qt Multimedia 内部冗余日志，只保留应用自己的输出
    QLoggingCategory::setFilterRules(QStringLiteral(
        "qt.multimedia.*=false\n"
        "qt.multimedia.ffmpeg=false\n"
        "default=true"));

    // 3. 启动应用
    QApplication app(argc, argv);
    VideoPlayer player;
    player.show();
    return app.exec();
}
