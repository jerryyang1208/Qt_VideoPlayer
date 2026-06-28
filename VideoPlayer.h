#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>
#include <QStandardItemModel>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class VideoPlayer; }
QT_END_NAMESPACE

// 音视频播放器主窗口
class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    // 播放模式：顺序 / 随机 / 单曲循环
    enum PlayMode { Order, Random, RepeatOne };

    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

protected:
    // 拦截视频窗口关闭事件，改为隐藏而非销毁
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // —— 文件与列表 ——
    void openDirectory();                  // 打开文件夹，扫描音视频文件
    void onMusicListDoubleClicked(const QModelIndex &index);  // 双击列表播放

    // —— 播放控制 ——
    void prevSong();                       // 上一首
    void nextSong();                       // 下一首
    void playPause();                      // 播放/暂停切换
    void autoSwitchToNext();               // 播放结束自动切歌
    void onPlayModeClicked();              // 切换播放模式

    // —— 音量 ——
    void toggleVolume();                   // 显示/隐藏音量条
    void adjustVolume(int value);          // 调节音量（线性转对数）

    // —— UI 更新 ——
    void updatePlayButtonIcon(QMediaPlayer::PlaybackState state);
    void updateDurationLab(qint64 duration);     // 更新总时长
    void updatePlayDurLab(qint64 position);      // 更新当前进度
    void onCurrentMediaChanged(const QUrl &url); // 媒体切换时显示/隐藏视频窗口

private:
    void initVideoWindow();           // 初始化独立视频窗口
    void playAtIndex(int index);      // 播放列表中指定索引的文件
    void switchSong(int direction);   // 上/下一首公共实现，direction = -1/+1
    void updatePlayModeIcon();        // 根据当前模式更新播放模式按钮图标

    bool isVideoFile(const QString &filePath) const;  // 判断是否为视频文件
    int  currentIndex() const;                        // 当前播放索引（带边界校验）
    int  nextIndex(int direction) const;              // 按当前播放模式算出下一个索引
    QString currentFileName() const;                  // 当前播放文件的文件名
    static qreal linearToLogVolume(int linearVolume); // 线性音量 0-100 转对数音量 0.0-1.0

    Ui::VideoPlayer *ui;
    QStandardItemModel *m_listModel;    // 播放列表数据模型
    QMediaPlayer       *m_mediaPlayer;  // 媒体播放器
    QAudioOutput       *m_audioOutput;  // 音频输出
    QVideoWidget       *m_videoWidget;  // 视频显示控件
    QWidget            *m_videoWindow = nullptr;  // 独立视频窗口

    int       m_currentIndex = -1;      // 当前播放索引，-1 表示未播放
    PlayMode  m_playMode     = Order;   // 当前播放模式

    const QStringList m_supportedAudioFormats; // 支持的音频扩展名
    const QStringList m_supportedVideoFormats; // 支持的视频扩展名
};

#endif // VIDEOPLAYER_H
