#include "VideoPlayer.h"
#include "ui_VideoPlayer.h"

#include <QDirIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTime>
#include <QVBoxLayout>
#include <QtMath>
#include <QDebug>
#include <utility>

// 初始化流程：UI → 列表/图标 → 支持格式 → 视频窗口 → 媒体播放器 → 信号连接
VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoPlayer)
    , m_listModel(new QStandardItemModel(this))
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_videoWidget(new QVideoWidget(this))
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("音视频播放器"));
    ui->musicListView->setModel(m_listModel);
    ui->volumeBar->setVisible(false);
    ui->playBtn->setIcon(QIcon(QStringLiteral(":/Resource/play.png")));
    updatePlayModeIcon();

    m_supportedAudioFormats << "mp3" << "wav" << "ogg" << "flac" << "aac" << "m4a";
    m_supportedVideoFormats << "mp4" << "mkv" << "mov" << "flv" << "avi" << "wmv";

    initVideoWindow();

    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_mediaPlayer->setVideoOutput(m_videoWidget);

    // ---- 媒体信号 ----
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged,
            this, &VideoPlayer::updatePlayButtonIcon);

    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this,
            [this](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::EndOfMedia)
                    autoSwitchToNext();
            });

    connect(m_mediaPlayer, &QMediaPlayer::errorOccurred, this,
            [this](QMediaPlayer::Error error, const QString &errorString) {
                QMessageBox::warning(this, QStringLiteral("播放错误"),
                                     QStringLiteral("无法播放该文件：%1").arg(errorString));
                qWarning().noquote() << "媒体播放器错误：" << error << "-" << errorString;
                autoSwitchToNext();
            });

    connect(m_mediaPlayer, &QMediaPlayer::durationChanged,
            this, &VideoPlayer::updateDurationLab);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged,
            this, &VideoPlayer::updatePlayDurLab);
    connect(m_mediaPlayer, &QMediaPlayer::sourceChanged,
            this, &VideoPlayer::onCurrentMediaChanged);

    // ---- 按钮交互 ----
    connect(ui->openDirBtn,  &QPushButton::clicked, this, &VideoPlayer::openDirectory);
    connect(ui->playBtn,     &QPushButton::clicked, this, &VideoPlayer::playPause);
    connect(ui->prevBtn,     &QPushButton::clicked, this, &VideoPlayer::prevSong);
    connect(ui->nextBtn,     &QPushButton::clicked, this, &VideoPlayer::nextSong);
    connect(ui->volumeBtn,   &QPushButton::clicked, this, &VideoPlayer::toggleVolume);
    connect(ui->volumeBar,   &QSlider::sliderMoved, this, &VideoPlayer::adjustVolume);
    connect(ui->musicListView, &QListView::doubleClicked,
            this, &VideoPlayer::onMusicListDoubleClicked);
    connect(ui->playSlider,  &QSlider::sliderMoved, this, &VideoPlayer::seekPosition);
    connect(ui->playModeBtn, &QPushButton::clicked, this, &VideoPlayer::onPlayModeClicked);
}

VideoPlayer::~VideoPlayer()
{
    m_mediaPlayer->stop();
    m_mediaPlayer->setSource(QUrl());

    // 手动销毁视频组件，确保在父对象析构前释放 D3D 资源
    m_videoWidget->setParent(nullptr);
    delete m_videoWidget;
    delete ui;
}

void VideoPlayer::initVideoWindow()
{
    m_videoWindow = new QWidget();
    m_videoWindow->setWindowTitle(QStringLiteral("VideoPlayer"));
    m_videoWindow->resize(800, 600);

    auto *layout = new QVBoxLayout(m_videoWindow);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_videoWidget);

    m_videoWindow->installEventFilter(this);
    m_videoWindow->hide();
}

bool VideoPlayer::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_videoWindow && event->type() == QEvent::Close) {
        event->ignore();
        m_videoWindow->hide();
        if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState)
            m_mediaPlayer->pause();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

bool VideoPlayer::isVideoFile(const QString &filePath) const
{
    return m_supportedVideoFormats.contains(
        QFileInfo(filePath).suffix().toLower());
}

void VideoPlayer::onCurrentMediaChanged(const QUrl &url)
{
    if (!url.isEmpty())
        isVideoFile(url.toLocalFile()) ? m_videoWindow->show()
                                       : m_videoWindow->hide();
}

void VideoPlayer::updatePlayButtonIcon(QMediaPlayer::PlaybackState state)
{
    ui->playBtn->setIcon(QIcon(state == QMediaPlayer::PlayingState
        ? QStringLiteral(":/Resource/pause.png")
        : QStringLiteral(":/Resource/play.png")));
}

void VideoPlayer::updatePlayModeIcon()
{
    switch (m_playMode) {
    case Order:
        ui->playModeBtn->setIcon(QIcon(QStringLiteral(":/Resource/playlist_order.png")));
        ui->playModeBtn->setToolTip(QStringLiteral("顺序播放"));
        break;
    case Random:
        ui->playModeBtn->setIcon(QIcon(QStringLiteral(":/Resource/playlist_random.png")));
        ui->playModeBtn->setToolTip(QStringLiteral("随机播放"));
        break;
    case RepeatOne:
        ui->playModeBtn->setIcon(QIcon(QStringLiteral(":/Resource/playlist_repeat.png")));
        ui->playModeBtn->setToolTip(QStringLiteral("单曲循环"));
        break;
    }
}

// 播放列表索引的边界校验，返回有效索引或 -1
int VideoPlayer::currentIndex() const
{
    return (m_currentIndex >= 0 && m_currentIndex < m_listModel->rowCount())
               ? m_currentIndex : -1;
}

// 核心播放入口：设置源 + 更新列表选中 + 自动开始播放
void VideoPlayer::playAtIndex(int index)
{
    const QString filePath = m_listModel->index(index, 0)
                                 .data(Qt::UserRole + 1).toString();
    m_currentIndex = index;
    ui->musicListView->setCurrentIndex(m_listModel->index(index, 0));
    m_mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
    m_mediaPlayer->play();
}

// 「上一首 / 下一首」共用实现
void VideoPlayer::switchSong(int direction)
{
    const int idx   = currentIndex();
    const int count = m_listModel->rowCount();
    if (idx == -1 || count == 0) return;

    // 单曲循环 → 重播当前
    if (m_playMode == RepeatOne) {
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->play();
        qDebug().noquote() << "重播当前项:"
                           << QFileInfo(m_mediaPlayer->source().toLocalFile()).fileName();
        return;
    }

    // 计算下一个索引
    int newIdx = idx;
    if (m_playMode == Random) {
        if (count > 1) {
            do { newIdx = QRandomGenerator::global()->bounded(count); }
            while (newIdx == idx);
        }
    } else {
        newIdx = (idx + direction + count) % count;
    }

    if (newIdx == idx) {
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->play();
        qDebug().noquote() << "重播当前项:"
                           << QFileInfo(m_mediaPlayer->source().toLocalFile()).fileName();
        return;
    }

    playAtIndex(newIdx);
    qDebug().noquote() << "切换到" << (direction < 0 ? "上一" : "下一") << "项:"
                       << QFileInfo(m_mediaPlayer->source().toLocalFile()).fileName();
}

void VideoPlayer::prevSong() { switchSong(-1); }
void VideoPlayer::nextSong() { switchSong(+1); }

// 自动切歌：播放结束或出错时调用，根据播放模式决定下一首
void VideoPlayer::autoSwitchToNext()
{
    const int idx   = currentIndex();
    const int count = m_listModel->rowCount();
    if (idx == -1 || count == 0) return;

    int nextIdx = idx;
    switch (m_playMode) {
    case Order:     nextIdx = (idx + 1) % count; break;
    case RepeatOne: break;                       // 保持当前索引
    case Random:
        if (count > 1) {
            do { nextIdx = QRandomGenerator::global()->bounded(count); }
            while (nextIdx == idx);
        }
        break;
    }

    if (m_playMode == RepeatOne)
        m_mediaPlayer->stop();
    // 单曲循环时先 stop 再 play，确保从头播放
    playAtIndex(nextIdx);
    qDebug().noquote() << "自动切换到:"
                       << QFileInfo(m_mediaPlayer->source().toLocalFile()).fileName();
}

void VideoPlayer::openDirectory()
{
    const QString path = QFileDialog::getExistingDirectory(
        this, QStringLiteral("选择文件夹"), "D:/YXR/Media/Audio");
    if (path.isEmpty()) return;

    m_listModel->clear();
    m_currentIndex = -1;

    QStringList filters;
    for (const QString &fmt : std::as_const(m_supportedAudioFormats))
        filters << "*." + fmt;
    for (const QString &fmt : std::as_const(m_supportedVideoFormats))
        filters << "*." + fmt;

    QDirIterator it(path, filters);
    while (it.hasNext()) {
        it.next();
        const QFileInfo info = it.fileInfo();
        auto *item = new QStandardItem(info.fileName());
        item->setData(info.canonicalFilePath(), Qt::UserRole + 1);
        m_listModel->appendRow(item);
    }
}

void VideoPlayer::onMusicListDoubleClicked(const QModelIndex &index)
{
    playAtIndex(index.row());
    qDebug().noquote() << "选中播放:"
                       << QFileInfo(m_mediaPlayer->source().toLocalFile()).fileName();
}

void VideoPlayer::playPause()
{
    if (m_listModel->rowCount() == 0 || m_currentIndex == -1) return;

    if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState)
        m_mediaPlayer->pause();
    else
        m_mediaPlayer->play();
}

void VideoPlayer::onPlayModeClicked()
{
    m_playMode = static_cast<PlayMode>((m_playMode + 1) % 3);
    updatePlayModeIcon();
}

void VideoPlayer::seekPosition(int position) { m_mediaPlayer->setPosition(position); }

void VideoPlayer::toggleVolume()
{
    ui->volumeBar->setVisible(!ui->volumeBar->isVisible());
}

qreal VideoPlayer::linearToLogVolume(int linearVolume)
{
    if (linearVolume <= 0)   return 0.0;
    if (linearVolume >= 100) return 1.0;
    return qPow(linearVolume / 100.0, 1.8);
}

void VideoPlayer::adjustVolume(int value)
{
    m_audioOutput->setVolume(linearToLogVolume(value));
}

// 毫秒 → "HH:mm:ss" 格式化
static QString msToTimeString(qint64 ms)
{
    return QTime(0, 0).addMSecs(static_cast<int>(ms)).toString("00:mm:ss");
}

// 总时长变化时更新标签和进度条范围
void VideoPlayer::updateDurationLab(qint64 duration)
{
    ui->durationLab->setText(msToTimeString(duration));
    ui->playSlider->setRange(0, static_cast<int>(duration));
}

// 当前播放进度更新：更新标签 + 同步进度条
// 注意：用户拖动进度条时不更新，避免与 positionChanged 信号互相干扰
void VideoPlayer::updatePlayDurLab(qint64 position)
{
    ui->playDurLab->setText(msToTimeString(position));
    if (!ui->playSlider->isSliderDown())
        ui->playSlider->setValue(static_cast<int>(position));
}
