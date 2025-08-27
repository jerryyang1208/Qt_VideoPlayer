/********************************************************************************
** Form generated from reading UI file 'VideoPlayer.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOPLAYER_H
#define UI_VIDEOPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoPlayer
{
public:
    QSlider *volumeBar;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QListView *musicListView;
    QHBoxLayout *horizontalLayout_2;
    QLabel *playDurLab;
    QLabel *label_3;
    QLabel *durationLab;
    QSpacerItem *horizontalSpacer;
    QSlider *playSlider;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *openDirBtn;
    QPushButton *prevBtn;
    QPushButton *playBtn;
    QPushButton *nextBtn;
    QPushButton *volumeBtn;

    void setupUi(QWidget *VideoPlayer)
    {
        if (VideoPlayer->objectName().isEmpty())
            VideoPlayer->setObjectName("VideoPlayer");
        VideoPlayer->resize(473, 595);
        VideoPlayer->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	min-width:32px;\n"
"	min-height:32px;\n"
"	max-width:32px;\n"
"	max-height:32px;\n"
"	\n"
"	border:none;\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"	background-color:rgb(160,160,160);\n"
"}"));
        volumeBar = new QSlider(VideoPlayer);
        volumeBar->setObjectName("volumeBar");
        volumeBar->setEnabled(true);
        volumeBar->setGeometry(QRect(350, 560, 91, 20));
        volumeBar->setStyleSheet(QString::fromUtf8("QSlider#volumeBar {\n"
"    min-height: 20px; /* \347\241\256\344\277\235\351\232\220\350\227\217\346\227\266\344\273\215\345\215\240\346\215\256\347\251\272\351\227\264\357\274\214\351\201\277\345\205\215\345\270\203\345\261\200\350\267\263\345\212\250 */\n"
"}"));
        volumeBar->setMaximum(100);
        volumeBar->setValue(100);
        volumeBar->setSliderPosition(100);
        volumeBar->setOrientation(Qt::Orientation::Horizontal);
        layoutWidget = new QWidget(VideoPlayer);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(9, 9, 451, 541));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        musicListView = new QListView(layoutWidget);
        musicListView->setObjectName("musicListView");
        musicListView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);

        gridLayout->addWidget(musicListView, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        playDurLab = new QLabel(layoutWidget);
        playDurLab->setObjectName("playDurLab");

        horizontalLayout_2->addWidget(playDurLab);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        durationLab = new QLabel(layoutWidget);
        durationLab->setObjectName("durationLab");

        horizontalLayout_2->addWidget(durationLab);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        playSlider = new QSlider(layoutWidget);
        playSlider->setObjectName("playSlider");
        playSlider->setOrientation(Qt::Orientation::Horizontal);

        gridLayout->addWidget(playSlider, 2, 0, 1, 1);

        widget = new QWidget(layoutWidget);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        openDirBtn = new QPushButton(widget);
        openDirBtn->setObjectName("openDirBtn");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Resource/openDir.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        openDirBtn->setIcon(icon);

        horizontalLayout->addWidget(openDirBtn);

        prevBtn = new QPushButton(widget);
        prevBtn->setObjectName("prevBtn");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Resource/prevSong.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        prevBtn->setIcon(icon1);

        horizontalLayout->addWidget(prevBtn);

        playBtn = new QPushButton(widget);
        playBtn->setObjectName("playBtn");

        horizontalLayout->addWidget(playBtn);

        nextBtn = new QPushButton(widget);
        nextBtn->setObjectName("nextBtn");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Resource/nextSong.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        nextBtn->setIcon(icon2);

        horizontalLayout->addWidget(nextBtn);

        volumeBtn = new QPushButton(widget);
        volumeBtn->setObjectName("volumeBtn");
        volumeBtn->setStyleSheet(QString::fromUtf8("QPushButton#volumeBtn:hover + QSlider#volumeBar {\n"
"    visibility: visible;\n"
"}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Resource/volume.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        volumeBtn->setIcon(icon3);

        horizontalLayout->addWidget(volumeBtn);


        gridLayout->addWidget(widget, 3, 0, 1, 1);


        retranslateUi(VideoPlayer);

        QMetaObject::connectSlotsByName(VideoPlayer);
    } // setupUi

    void retranslateUi(QWidget *VideoPlayer)
    {
        VideoPlayer->setWindowTitle(QCoreApplication::translate("VideoPlayer", "MusicPlayer", nullptr));
#if QT_CONFIG(tooltip)
        volumeBar->setToolTip(QCoreApplication::translate("VideoPlayer", "\351\237\263\351\207\217\346\235\241", nullptr));
#endif // QT_CONFIG(tooltip)
        playDurLab->setText(QCoreApplication::translate("VideoPlayer", "00:00:00", nullptr));
        label_3->setText(QCoreApplication::translate("VideoPlayer", "/", nullptr));
        durationLab->setText(QCoreApplication::translate("VideoPlayer", "00:00:00", nullptr));
#if QT_CONFIG(tooltip)
        playSlider->setToolTip(QCoreApplication::translate("VideoPlayer", "\346\222\255\346\224\276\350\277\233\345\272\246\346\235\241", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        openDirBtn->setToolTip(QCoreApplication::translate("VideoPlayer", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", nullptr));
#endif // QT_CONFIG(tooltip)
        openDirBtn->setText(QString());
#if QT_CONFIG(tooltip)
        prevBtn->setToolTip(QCoreApplication::translate("VideoPlayer", "\344\270\212\344\270\200\351\241\271", nullptr));
#endif // QT_CONFIG(tooltip)
        prevBtn->setText(QString());
#if QT_CONFIG(tooltip)
        playBtn->setToolTip(QCoreApplication::translate("VideoPlayer", "\346\222\255\346\224\276", nullptr));
#endif // QT_CONFIG(tooltip)
        playBtn->setText(QString());
#if QT_CONFIG(tooltip)
        nextBtn->setToolTip(QCoreApplication::translate("VideoPlayer", "\344\270\213\344\270\200\351\241\271", nullptr));
#endif // QT_CONFIG(tooltip)
        nextBtn->setText(QString());
#if QT_CONFIG(tooltip)
        volumeBtn->setToolTip(QCoreApplication::translate("VideoPlayer", "\351\237\263\351\207\217", nullptr));
#endif // QT_CONFIG(tooltip)
        volumeBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VideoPlayer: public Ui_VideoPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOPLAYER_H
