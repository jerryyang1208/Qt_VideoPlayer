#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QTimer>
#include <QVideoWidget>  // 视频组件头文件
#include <QWidget>       // 用于视频窗口

QT_BEGIN_NAMESPACE
namespace Ui {
class VideoPlayer;
}
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    // 播放模式枚举
    enum PlayMode {
        Order,      // 顺序播放
        Random,     // 随机播放
        RepeatOne   // 单曲循环
    };

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void openDirectory();
    void onMusicListDoubleClicked(const QModelIndex &index);
    void prevSong();
    void nextSong();
    void toggleVolume();
    void adjustVolume(int value);
    void playPause();
    void updatePlayButtonIcon(QMediaPlayer::PlaybackState state);
    void updateDurationLab(qint64 duration);
    void updatePlayDurLab(qint64 position);
    void seekPosition(int position);
    void autoSwitchToNext();
    void checkPlayCompletion();
    void onCurrentMediaChanged(const QUrl &url); // 处理当前媒体文件变化
    void onPlayModeClicked();  // 播放模式按钮点击切换

private:
    Ui::VideoPlayer *ui;
    QStandardItemModel* m_listModel;
    QMediaPlayer* m_mediaPlayer{};
    QAudioOutput* m_audioOutput{};
    QVideoWidget *m_videoWidget;  // 视频显示组件
    QWidget *m_videoWindow;       // 独立视频窗口
    QSlider* volumeSlider;
    QWidget* volumeWidget; // 用于容纳音量滑块的浮动窗口
    QTimer* m_timer;       // 用于定时更新进度条
    qreal linearToLogVolume(int linearVolume);
    int currentSongIndex;  // 当前播放歌曲的索引
    int getCurrentSongIndex() const;
    bool m_isAutoSwitch;   // 标记是否为自动切歌（非手动操作）
    bool m_isPlayCompleted;
    bool m_isVideoPlaying;  // 标记当前是否是在播放视频
    PlayMode m_playMode;       // 当前播放模式
    void updatePlayModeIcon(); // 更新播放模式按钮图标

    // 支持的音视频格式
    QStringList m_supportedAudioFormats;
    QStringList m_supportedVideoFormats;

    void initVideoWindow(); // 初始化视频窗口
    bool isVideoFile(const QString &filePath);     // 判断文件是否为视频
    void updateVideoWindowVisibility(bool isVideo);// 根据文件类型显示/隐藏视频窗口

};
#endif // VIDEOPLAYER_H
