#include <QApplication>
#include "VideoPlayer.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    // 设置控制台输出代码页为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    QApplication a(argc, argv);
    VideoPlayer w;
    w.show();
    return a.exec();
}
