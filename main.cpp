#include <QApplication>

#include <QUrl>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaDevices>
#include <QAudio>
#include <QAudioOutput>
#include <QAudioDevice>
#include <QList>

#include "tool.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //播放器和视频小部件
    QMediaPlayer *player = new QMediaPlayer;
    QVideoWidget *video = new QVideoWidget;

    player->setVideoOutput(video);
    player->setSource(QUrl("qrc:/milk.mp4"));

    // 全屏
    video->showFullScreen();

    // 禁用鼠标 任务栏 键盘
    ShowCursor(false);

//    MyThread work;
//    work.start();

//    ::ShowWindow(::FindWindow(L"Shell_TrayWnd",NULL),SW_HIDE);
//    ::ShowWindow(::FindWindow(L"Shell_TrayWnd",NULL),SW_SHOW);

    DisableKeyboard();

    //设置系统音量
//    SetMute(0);
//    SetMasterVolume(1.0f);


    //指定扬声器播放
    QList<QAudioDevice> devs=QMediaDevices::audioOutputs();
    QAudioDevice dev;
    dev=devs.first();
    for (const QAudioDevice &device : devs)
    {
        qDebug() << "Device: " << device.description();
        if(device.description().contains("扬声器")){
            dev=device;
        }
    }
    QAudioOutput out(dev);out.setVolume(1.0f);
    player->setAudioOutput(&out);

    player->setPlaybackRate(1.5);

    //循环播放
    player->setLoops(QMediaPlayer::Infinite);

//    player->play();

    return a.exec();
}
