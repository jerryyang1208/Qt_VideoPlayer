#include <QApplication>
#include "VideoPlayer.h"
#include <QDebug>
#include <windows.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// 自定义消息处理器，解决中文乱码问题
void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // 获取当前控制台代码页
    UINT originalCodePage = GetConsoleOutputCP();

    // 临时设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    // 输出到控制台（使用 UTF-8 编码）
    QByteArray localMsg = msg.toUtf8();

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s\n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", localMsg.constData());
        abort();
    }

    // 恢复原来的代码页
    SetConsoleOutputCP(originalCodePage);

    // 同时输出到 VS 输出窗口（可选）
    OutputDebugStringW(msg.toStdWString().c_str());
}

int main(int argc, char* argv[])
{
    // 安装自定义消息处理器
    qInstallMessageHandler(customMessageHandler);

    // 设置控制台输出代码页为 UTF-8
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
#endif

    QApplication a(argc, argv);
    VideoPlayer w;
    w.show();
    return a.exec();
}
