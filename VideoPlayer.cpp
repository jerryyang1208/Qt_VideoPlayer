#include "VideoPlayer.h"
#include "ui_VideoPlayer.h"
#include <QFileDialog>
#include <QDirIterator>
#include <QDebug>
#include <QFileInfo>
#include <QVBoxLayout>
#include <utility>
#include <QTime>
#include <cstdlib>
#include <ctime>
#include <QtMath>

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoPlayer)
    , m_listModel(new QStandardItemModel(this))
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_videoWidget(new QVideoWidget(this))
    , currentSongIndex(-1)
    , m_isAutoSwitch(false)
    , m_isPlayCompleted(false)
    , m_isVideoPlaying(false)
    , m_timer(new QTimer(this))
    , m_playMode(Order)
{
    ui->setupUi(this);
    setWindowTitle("音视频播放器");
    ui->musicListView->setModel(m_listModel);
    ui->volumeBar->setVisible(false);
    ui->playBtn->setIcon(QIcon(":/Resource/play.png"));
    updatePlayModeIcon();

    // 初始化随机数种子（仅一次）
    srand(static_cast<unsigned>(QTime::currentTime().msecsSinceStartOfDay()));

    // 初始化支持的音视频格式
    m_supportedAudioFormats << "mp3" << "wav" << "ogg" << "flac" << "aac" << "m4a";
    m_supportedVideoFormats << "mp4" << "mkv" << "mov" << "flv" << "avi" << "wmv";

    // 初始化视频窗口
    initVideoWindow();

    // 配置媒体播放器
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_mediaPlayer->setVideoOutput(m_videoWidget);

    // 播放状态变化更新按钮图标和定时器
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        updatePlayButtonIcon(state);
        if (state == QMediaPlayer::PlayingState) {
            m_timer->start(1000);      // 播放时每秒检查一次时间
            m_isPlayCompleted = false; // 播放状态时重置完成标记
        } else {
            m_timer->stop();
        }
    });

    // 其他信号连接
    connect(m_timer, &QTimer::timeout, this, &VideoPlayer::checkPlayCompletion);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::updateDurationLab);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::updatePlayDurLab);

    // 连接当前媒体变化信号
    connect(m_mediaPlayer, &QMediaPlayer::sourceChanged, this, &VideoPlayer::onCurrentMediaChanged);

    // 按钮交互连接
    connect(ui->openDirBtn, &QPushButton::clicked, this, &VideoPlayer::openDirectory);
    connect(ui->playBtn, &QPushButton::clicked, this, &VideoPlayer::playPause);
    connect(ui->prevBtn, &QPushButton::clicked, this, &VideoPlayer::prevSong);
    connect(ui->nextBtn, &QPushButton::clicked, this, &VideoPlayer::nextSong);
    connect(ui->volumeBtn, &QPushButton::clicked, this, &VideoPlayer::toggleVolume);
    connect(ui->volumeBar, &QSlider::sliderMoved, this, &VideoPlayer::adjustVolume);
    connect(ui->musicListView, &QListView::doubleClicked, this, &VideoPlayer::onMusicListDoubleClicked);
    connect(ui->playSlider, &QSlider::sliderMoved, this, &VideoPlayer::seekPosition);
    connect(ui->playModeBtn, &QPushButton::clicked, this, &VideoPlayer::onPlayModeClicked);
}

VideoPlayer::~VideoPlayer() {
    // 停止播放并清除资源
    if (m_mediaPlayer) {
        m_mediaPlayer->stop();
        m_mediaPlayer->setSource(QUrl()); // 清除源
    }
    // 手动销毁视频组件（确保在父对象销毁前释放D3D资源）
    if (m_videoWidget) {
        m_videoWidget->setParent(nullptr);
        delete m_videoWidget;
        m_videoWidget = nullptr;
    }
    delete ui;
}

// 初始化视频窗口
void VideoPlayer::initVideoWindow()
{
    // 创建独立的视频窗口
    m_videoWindow = new QWidget();
    m_videoWindow->setWindowTitle("VideoPlayer");
    m_videoWindow->resize(800, 600); // 设置默认大小

    // 创建布局并添加视频组件
    QVBoxLayout *layout = new QVBoxLayout(m_videoWindow);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_videoWidget);

    // 安装事件过滤器监控视频窗口事件
    m_videoWindow->installEventFilter(this);

    // 默认隐藏视频窗口
    m_videoWindow->hide();
}

// 实现事件过滤器处理视频窗口关闭事件
bool VideoPlayer::eventFilter(QObject *watched, QEvent *event)
{
    // 判断是否是视频窗口的关闭事件
    if (watched == m_videoWindow && event->type() == QEvent::Close) {
        // 阻止窗口真正关闭，改为隐藏
        event->ignore();
        m_videoWindow->hide();

        // 暂停播放
        if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
            m_mediaPlayer->pause();
        }

        // 更新播放状态标记
        m_isVideoPlaying = false;

        return true; // 表示事件已处理
    }

    // 其他事件交给父类处理
    return QWidget::eventFilter(watched, event);
}

// 判断文件是否为视频
bool VideoPlayer::isVideoFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();
    return m_supportedVideoFormats.contains(suffix);
}

// 根据文件类型显示/隐藏视频窗口
void VideoPlayer::updateVideoWindowVisibility(bool isVideo)
{
    m_isVideoPlaying = isVideo;
    if (isVideo) {
        m_videoWindow->show();
    } else {
        m_videoWindow->hide();
    }
}

// 处理当前媒体变化
void VideoPlayer::onCurrentMediaChanged(const QUrl &url)
{
    if (url.isEmpty()) return;

    QString filePath = url.toLocalFile();
    bool video = isVideoFile(filePath);
    updateVideoWindowVisibility(video);
}

// 更新播放按钮图标
void VideoPlayer::updatePlayButtonIcon(QMediaPlayer::PlaybackState state)
{
    if (state == QMediaPlayer::PlayingState) {
        ui->playBtn->setIcon(QIcon(":/Resource/pause.png"));
    } else {
        ui->playBtn->setIcon(QIcon(":/Resource/play.png"));
    }
}

// 更新播放模式按钮图标
void VideoPlayer::updatePlayModeIcon()
{
    switch (m_playMode) {
    case Order:
        ui->playModeBtn->setIcon(QIcon(":/Resource/playlist_order.png"));
        ui->playModeBtn->setToolTip("顺序播放");
        break;
    case Random:
        ui->playModeBtn->setIcon(QIcon(":/Resource/playlist_random.png"));
        ui->playModeBtn->setToolTip("随机播放");
        break;
    case RepeatOne:
        ui->playModeBtn->setIcon(QIcon(":/Resource/playlist_repeat.png"));
        ui->playModeBtn->setToolTip("单曲循环");
        break;
    }
}

// 打开音视频文件夹
void VideoPlayer::openDirectory()
{
    auto path = QFileDialog::getExistingDirectory(this, "选择文件夹", "E:/desktop/YXR/Audio_video_tech/QT/Qt_Projects/Media");
    if (path.isEmpty()) return;

    // 清空现有列表
    m_listModel->clear();
    currentSongIndex = -1;

    // 创建文件过滤器，包含所有支持的音视频格式
    QStringList filters;
    for (const QString &format : std::as_const(m_supportedAudioFormats)) {
        filters << "*." + format;
    }
    for (const QString &format : std::as_const(m_supportedVideoFormats)) {
        filters << "*." + format;
    }

    QDirIterator it(path, filters);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        auto item = new QStandardItem(info.fileName());
        item->setData(it.fileInfo().canonicalFilePath(), Qt::UserRole + 1);
        m_listModel->appendRow(item);
    }
}

// 双击列表播放音视频
void VideoPlayer::onMusicListDoubleClicked(const QModelIndex &index)
{
    auto filePath = index.data(Qt::UserRole + 1).toString();
    auto url = QUrl::fromLocalFile(filePath);
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    m_mediaPlayer->setSource(url);
    m_mediaPlayer->play();
    ui->musicListView->setCurrentIndex(index);
    currentSongIndex = index.row();
    m_isPlayCompleted = false;  // 重置完成标记

    // 强制检查文件类型并显示视频窗口
    bool isVideo = isVideoFile(filePath);
    if (isVideo) {
        m_videoWindow->show();
        m_isVideoPlaying = true;
    }

    qDebug().noquote() << "选中播放:" << fileName.toUtf8().constData();
}

// 获取当前播放索引
int VideoPlayer::getCurrentSongIndex() const
{
    if (currentSongIndex < 0 || currentSongIndex >= m_listModel->rowCount()) {
        return -1;
    }
    return currentSongIndex;
}

// 播放/暂停按钮
void VideoPlayer::playPause()
{
    if (m_listModel->rowCount() == 0 || currentSongIndex == -1) return;

    if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        m_mediaPlayer->pause();
    } else {
        m_mediaPlayer->play();
        // 播放时检查是否为视频文件，若是则显示视频窗口
        QString currentPath = m_mediaPlayer->source().toLocalFile();
        if (isVideoFile(currentPath)) {
            m_videoWindow->show();
            m_isVideoPlaying = true;
        }
    }
}

// 播放模式按钮
void VideoPlayer::onPlayModeClicked()
{
    // 切换播放模式（循环切换）
    m_playMode = static_cast<PlayMode>((m_playMode + 1) % 3);
    updatePlayModeIcon();
}

// 切换上一项按钮
void VideoPlayer::prevSong()
{
    int currentIndex = getCurrentSongIndex();
    int rowCount = m_listModel->rowCount();
    if (currentIndex == -1 || rowCount == 0) return;

    if (m_playMode == RepeatOne) {
        // 单曲循环：重播当前
        QString filePath = m_listModel->index(currentIndex, 0).data(Qt::UserRole + 1).toString();
        QFileInfo info(filePath);
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->play();
        if (isVideoFile(filePath)) {
            m_videoWindow->show();
            m_isVideoPlaying = true;
        }
        m_isPlayCompleted = false;
        qDebug().noquote() << "重播当前项:" << info.fileName().toUtf8().constData();
        return;
    }

    int newIndex;
    if (m_playMode == Random) {
        if (rowCount == 1) {
            newIndex = currentIndex;  // 仅一首，重播
        } else {
            do {
                newIndex = rand() % rowCount;
            } while (newIndex == currentIndex);
        }
    } else { // Order
        newIndex = (currentIndex - 1 + rowCount) % rowCount;
    }

    // 如果随机模式下选中了当前（仅一首的情况），也按重播处理
    if (newIndex == currentIndex) {
        QString filePath = m_listModel->index(currentIndex, 0).data(Qt::UserRole + 1).toString();
        QFileInfo info(filePath);
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->play();
        if (isVideoFile(filePath)) {
            m_videoWindow->show();
            m_isVideoPlaying = true;
        }
        m_isPlayCompleted = false;
        qDebug().noquote() << "重播当前项:" << info.fileName().toUtf8().constData();
        return;
    }

    // 切换到新索引
    QModelIndex index = m_listModel->index(newIndex, 0);
    QString filePath = index.data(Qt::UserRole + 1).toString();
    QUrl url = QUrl::fromLocalFile(filePath);
    QFileInfo info(filePath);

    m_mediaPlayer->setSource(url);
    m_mediaPlayer->play();
    currentSongIndex = newIndex;
    ui->musicListView->setCurrentIndex(index);
    m_isPlayCompleted = false;
    qDebug().noquote() << "切换到上一项:" << info.fileName().toUtf8().constData();
}

// 切换下一项按钮
void VideoPlayer::nextSong()
{
    int currentIndex = getCurrentSongIndex();
    int rowCount = m_listModel->rowCount();
    if (currentIndex == -1 || rowCount == 0) return;

    if (m_playMode == RepeatOne) {
        // 单曲循环：重播当前
        QString filePath = m_listModel->index(currentIndex, 0).data(Qt::UserRole + 1).toString();
        QFileInfo info(filePath);
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->play();
        if (isVideoFile(filePath)) {
            m_videoWindow->show();
            m_isVideoPlaying = true;
        }
        m_isPlayCompleted = false;
        qDebug().noquote() << "重播当前项:" << info.fileName().toUtf8().constData();
        return;
    }

    int newIndex;
    if (m_playMode == Random) {
        if (rowCount == 1) {
            newIndex = currentIndex;
        } else {
            do {
                newIndex = rand() % rowCount;
            } while (newIndex == currentIndex);
        }
    } else { // Order
        newIndex = (currentIndex + 1) % rowCount;
    }

    if (newIndex == currentIndex) {
        QString filePath = m_listModel->index(currentIndex, 0).data(Qt::UserRole + 1).toString();
        QFileInfo info(filePath);
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->play();
        if (isVideoFile(filePath)) {
            m_videoWindow->show();
            m_isVideoPlaying = true;
        }
        m_isPlayCompleted = false;
        qDebug().noquote() << "重播当前项:" << info.fileName().toUtf8().constData();
        return;
    }

    QModelIndex index = m_listModel->index(newIndex, 0);
    QString filePath = index.data(Qt::UserRole + 1).toString();
    QUrl url = QUrl::fromLocalFile(filePath);
    QFileInfo info(filePath);

    m_mediaPlayer->setSource(url);
    m_mediaPlayer->play();
    currentSongIndex = newIndex;
    ui->musicListView->setCurrentIndex(index);
    m_isPlayCompleted = false;

    if (!m_isAutoSwitch) {
        qDebug().noquote() << "切换到下一项:" << info.fileName().toUtf8().constData();
    }
}

// 音量按钮
void VideoPlayer::toggleVolume()
{
    ui->volumeBar->setVisible(!ui->volumeBar->isVisible());
}

// 音量线性转对数
qreal VideoPlayer::linearToLogVolume(int linearVolume)
{
    if (linearVolume <= 0)
        return 0.0;

    if (linearVolume >= 100)
        return 1.0;

    // 使用1.8次方曲线，使音量变化更自然
    // 滑块值50时返回约0.28，接近主流播放器的中间音量
    qreal normalized = linearVolume / 100.0;
    return qPow(normalized, 1.8);
}

// 音量条拖动
void VideoPlayer::adjustVolume(int value)
{
    m_audioOutput->setVolume(linearToLogVolume(value));
}

void VideoPlayer::updateDurationLab(qint64 duration)
{
    QTime totalTime(0, 0);
    totalTime = totalTime.addMSecs(duration);
    ui->durationLab->setText(totalTime.toString("00:mm:ss")); // 总时长标签
    ui->playSlider->setRange(0, duration);
}

void VideoPlayer::updatePlayDurLab(qint64 position)
{
    QTime currentTime(0, 0);
    currentTime = currentTime.addMSecs(position);
    ui->playDurLab->setText(currentTime.toString("00:mm:ss")); // 已播放时长标签
    if (!ui->playSlider->isSliderDown()) { // 避免拖动时冲突
        ui->playSlider->setValue(position);
    }
}

// 进度条拖动
void VideoPlayer::seekPosition(int position)
{
    m_mediaPlayer->setPosition(position);
}

// 检查是否播放完成，然后自动切歌
void VideoPlayer::checkPlayCompletion()
{
    // 获取当前播放位置和总时长（用毫秒数比较，更准确）
    qint64 currentPos = m_mediaPlayer->position();
    qint64 totalDur = m_mediaPlayer->duration();

    // 完播判定条件：已播放时长 >= 总时长（允许 1 秒误差），且未触发过自动切歌
    if (currentPos >= totalDur - 1000 && totalDur > 0 && !m_isPlayCompleted) {
        m_isPlayCompleted = true;  // 标记为已完成，避免重复触发
        autoSwitchToNext();        // 执行自动切歌
    }
}

// 自动切换到下一曲
void VideoPlayer::autoSwitchToNext()
{
    int currentIndex = getCurrentSongIndex();
    if (currentIndex == -1 || m_listModel->rowCount() == 0) return;

    int nextIndex = currentIndex;  // 默认保持当前索引

    switch (m_playMode) {
    case Order:
        // 顺序播放：切换到下一首，最后一首结束后回到第一首
        nextIndex = (currentIndex + 1) % m_listModel->rowCount();
        break;

    case Random:
        if (m_listModel->rowCount() > 1) {
            // 不再重新设置种子
            do {
                nextIndex = rand() % m_listModel->rowCount();
            } while (nextIndex == currentIndex);
        }
        break;

    case RepeatOne:
        // 单曲循环：保持当前索引
        nextIndex = currentIndex;
        break;
    }

    QModelIndex nextModelIndex = m_listModel->index(nextIndex, 0);
    ui->musicListView->setCurrentIndex(nextModelIndex);

    QString nextFilePath = nextModelIndex.data(Qt::UserRole + 1).toString();
    QFileInfo fileInfo(nextFilePath);
    QString fileName = fileInfo.fileName();

    m_isAutoSwitch = true;
    // 单曲循环时需要先停止当前播放再重新开始
    if (m_playMode == RepeatOne) {
        m_mediaPlayer->stop();
    }
    m_mediaPlayer->setSource(QUrl::fromLocalFile(nextFilePath));
    m_mediaPlayer->play();
    currentSongIndex = nextIndex;
    m_isAutoSwitch = false;

    qDebug().noquote() << "自动切换到:" << fileName.toUtf8().constData();
}
