#include "VideoPlayer.h"
#include "ui_VideoPlayer.h"

#include <QDirIterator>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QPixmap>
#include <QRandomGenerator>
#include <QShortcut>
#include <QTime>
#include <QVBoxLayout>
#include <QtMath>
#include <QDebug>

// 初始化流程：UI → 列表/图标 → 支持格式 → 视频窗口 → 媒体播放器 → 信号连接
VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoPlayer)
    , m_listModel(new QStandardItemModel(this))
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_videoWidget(new QVideoWidget(this))
    , m_supportedAudioFormats({"mp3", "wav", "ogg", "flac", "aac", "m4a"})
    , m_supportedVideoFormats({"mp4", "mkv", "mov", "flv", "avi", "wmv"})
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("音视频播放器"));
    ui->musicListView->setModel(m_listModel);
    ui->volumeBar->setVisible(false);
    ui->playBtn->setIcon(QIcon(QStringLiteral(":/Resource/play.png")));
    updatePlayModeIcon();
    adjustVolume(ui->volumeBar->value());  // 初始化音量（滑块默认 100 = 最大音量）

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

                // 连续错误超过列表长度则停止，避免所有文件都损坏时死循环
                if (++m_errorSwitchCount >= m_listModel->rowCount()) {
                    qWarning() << "连续错误已达上限，停止自动切歌";
                    m_errorSwitchCount = 0;
                    return;
                }
                autoSwitchToNext();
            });

    connect(m_mediaPlayer, &QMediaPlayer::durationChanged,
            this, &VideoPlayer::updateDurationLab);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged,
            this, &VideoPlayer::updatePlayDurLab);
    connect(m_mediaPlayer, &QMediaPlayer::sourceChanged,
            this, &VideoPlayer::onCurrentMediaChanged);
    connect(m_mediaPlayer, &QMediaPlayer::metaDataChanged,
            this, &VideoPlayer::updateCoverArt);

    // ---- 快捷键 ----
    auto *muteShortcut = new QShortcut(QKeySequence(Qt::Key_M), this);
    connect(muteShortcut, &QShortcut::activated, this, &VideoPlayer::toggleMute);
    auto *fsShortcut = new QShortcut(QKeySequence(Qt::Key_F), this);
    connect(fsShortcut, &QShortcut::activated, this, &VideoPlayer::toggleFullscreen);

    // ---- 按钮交互 ----
    connect(ui->openDirBtn,    &QPushButton::clicked,     this, &VideoPlayer::openDirectory);
    connect(ui->playBtn,       &QPushButton::clicked,     this, &VideoPlayer::playPause);
    connect(ui->prevBtn,       &QPushButton::clicked,     this, &VideoPlayer::prevSong);
    connect(ui->nextBtn,       &QPushButton::clicked,     this, &VideoPlayer::nextSong);
    connect(ui->volumeBtn,     &QPushButton::clicked,     this, &VideoPlayer::toggleVolume);
    connect(ui->volumeBar,     &QSlider::sliderMoved,     this, &VideoPlayer::adjustVolume);
    connect(ui->playModeBtn,   &QPushButton::clicked,     this, &VideoPlayer::onPlayModeClicked);
    connect(ui->musicListView, &QListView::doubleClicked, this, &VideoPlayer::onMusicListDoubleClicked);
    connect(ui->playSlider,  &QSlider::sliderMoved, m_mediaPlayer, &QMediaPlayer::setPosition);
}

VideoPlayer::~VideoPlayer()
{
    m_mediaPlayer->stop();
    m_mediaPlayer->setSource(QUrl());

    // 手动销毁视频组件，确保在父对象析构前释放 D3D 资源
    m_videoWidget->setParent(nullptr);
    delete m_videoWidget;
    delete m_videoWindow;
    delete ui;
}

void VideoPlayer::initVideoWindow()
{
    m_videoWindow = new QWidget();
    m_videoWindow->setWindowTitle(QStringLiteral("VideoPlayer"));
    m_videoWindow->resize(800, 600);

    // 封面图显示标签（音频播放时显示，视频播放时隐藏）
    m_coverLabel = new QLabel(m_videoWindow);
    m_coverLabel->setAlignment(Qt::AlignCenter);
    m_coverLabel->setScaledContents(true);
    m_coverLabel->setStyleSheet(QStringLiteral("background-color: black;"));
    m_coverLabel->hide();

    auto *layout = new QVBoxLayout(m_videoWindow);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_videoWidget);
    layout->addWidget(m_coverLabel);

    m_videoWindow->installEventFilter(this);
    m_videoWindow->hide();
}

bool VideoPlayer::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != m_videoWindow)
        return QWidget::eventFilter(watched, event);

    // 视频窗口关闭 → 隐藏而非销毁
    if (event->type() == QEvent::Close) {
        event->ignore();
        m_videoWindow->hide();
        if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState)
            m_mediaPlayer->pause();
        return true;
    }

    // 视频窗口按键：F 全屏切换 / ESC 退出全屏
    if (event->type() == QEvent::KeyPress) {
        auto *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_F) {
            toggleFullscreen();
            return true;
        }
        if (ke->key() == Qt::Key_Escape && m_videoWindow->isFullScreen()) {
            m_videoWindow->showNormal();
            return true;
        }
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
    if (url.isEmpty()) {
        m_videoWindow->hide();
        return;
    }

    if (isVideoFile(url.toLocalFile())) {
        // 视频：显示视频控件，隐藏封面
        m_videoWidget->show();
        m_coverLabel->hide();
    } else {
        // 音频：显示封面标签，隐藏视频控件，尝试加载封面
        m_videoWidget->hide();
        m_coverLabel->show();
        updateCoverArt();
    }
    m_videoWindow->show();
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

// 当前播放文件的文件名（供日志用）
QString VideoPlayer::currentFileName() const
{
    return QFileInfo(m_mediaPlayer->source().toLocalFile()).fileName();
}

// 按当前播放模式算出下一个播放索引
// direction = +1 下一首，-1 上一首
// 单曲循环模式始终返回当前索引
int VideoPlayer::nextIndex(int direction) const
{
    const int idx   = currentIndex();
    const int count = m_listModel->rowCount();
    if (idx == -1 || count == 0) return -1;

    switch (m_playMode) {
    case Order:
        return (idx + direction + count) % count;
    case RepeatOne:
        return idx;
    case Random:
        if (count <= 1) return idx;
        int next;
        do { next = QRandomGenerator::global()->bounded(count); }
        while (next == idx);
        return next;
    }
    return -1;
}

// 核心播放入口：设置源 + 更新列表选中 + 自动开始播放
void VideoPlayer::playAtIndex(int index)
{
    const QString filePath = m_listModel->index(index, 0)
                                 .data(Qt::UserRole + 1).toString();
    if (filePath.isEmpty()) {
        qWarning() << "播放失败：索引" << index << "对应的文件路径为空";
        return;
    }

    m_currentIndex = index;
    m_errorSwitchCount = 0;  // 正常播放时重置错误计数
    ui->musicListView->setCurrentIndex(m_listModel->index(index, 0));
    m_mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
    m_mediaPlayer->play();
}

// 「上一首 / 下一首」共用实现
// 单曲循环或索引未变时重播当前，否则切到下一首
void VideoPlayer::switchSong(int direction)
{
    const int idx = currentIndex();
    if (idx == -1) return;

    const int newIdx = nextIndex(direction);

    if (newIdx == idx) {
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->play();
        qDebug().noquote() << "重播当前项:" << currentFileName();
        return;
    }

    playAtIndex(newIdx);
    qDebug().noquote() << "切换到" << (direction < 0 ? "上一" : "下一") << "项:"
                       << currentFileName();
}

void VideoPlayer::prevSong() { switchSong(-1); }
void VideoPlayer::nextSong() { switchSong(+1); }

// 自动切歌：播放结束或出错时调用
void VideoPlayer::autoSwitchToNext()
{
    const int idx = currentIndex();
    if (idx == -1) return;

    const int nextIdx = nextIndex(+1);

    // 单曲循环时先 stop 再 play，确保从头播放
    if (m_playMode == RepeatOne)
        m_mediaPlayer->stop();

    playAtIndex(nextIdx);
    qDebug().noquote() << "自动切换到:" << currentFileName();
}

void VideoPlayer::openDirectory()
{
    const QString path = QFileDialog::getExistingDirectory(
        this, QStringLiteral("选择文件夹"), QDir::homePath());
    if (path.isEmpty()) return;

    m_mediaPlayer->stop();
    m_mediaPlayer->setSource(QUrl());
    m_listModel->clear();
    m_currentIndex = -1;

    QStringList filters;
    for (const QString &fmt : m_supportedAudioFormats)
        filters << "*." + fmt;
    for (const QString &fmt : m_supportedVideoFormats)
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
    qDebug().noquote() << "选中播放:" << currentFileName();
}

void VideoPlayer::playPause()
{
    if (currentIndex() == -1) return;

    if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        m_mediaPlayer->pause();
    } else {
        // 恢复播放时确保窗口可见（用户可能之前手动关闭了窗口）
        m_videoWindow->show();
        m_mediaPlayer->play();
    }
}

void VideoPlayer::onPlayModeClicked()
{
    m_playMode = static_cast<PlayMode>((m_playMode + 1) % 3);
    updatePlayModeIcon();
}

void VideoPlayer::toggleVolume()
{
    ui->volumeBar->setVisible(!ui->volumeBar->isVisible());
}

// 静音切换（快捷键 M）
void VideoPlayer::toggleMute()
{
    const bool muted = !m_audioOutput->isMuted();
    m_audioOutput->setMuted(muted);
    ui->volumeBtn->setToolTip(muted ? QStringLiteral("已静音") : QStringLiteral("音量"));
    qDebug().noquote() << (muted ? "已静音" : "已取消静音");
}

// 视频窗口全屏切换（快捷键 F）
void VideoPlayer::toggleFullscreen()
{
    if (!m_videoWindow || m_videoWindow->isHidden())
        return;

    if (m_videoWindow->isFullScreen())
        m_videoWindow->showNormal();
    else
        m_videoWindow->showFullScreen();
}

// 读取音频文件内嵌封面图并显示
void VideoPlayer::updateCoverArt()
{
    if (!m_coverLabel || isVideoFile(m_mediaPlayer->source().toLocalFile()))
        return;

    // 尝试从元数据中获取封面图（优先 CoverArtImage，其次 ThumbnailImage）
    const QMediaMetaData meta = m_mediaPlayer->metaData();
    QImage image = meta.value(QMediaMetaData::CoverArtImage).value<QImage>();
    if (image.isNull())
        image = meta.value(QMediaMetaData::ThumbnailImage).value<QImage>();

    if (!image.isNull()) {
        m_coverLabel->setPixmap(QPixmap::fromImage(image));
        qDebug().noquote() << "封面图已加载:" << currentFileName();
    } else {
        m_coverLabel->setText(QStringLiteral("无封面"));
    }
}

// 线性滑块值 → 感知音量：幂律曲线近似人耳非线性响度感知
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
