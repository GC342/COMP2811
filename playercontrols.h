#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include "the_player.h"



class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    PlayerControls(QWidget* parent);
    ~PlayerControls(){};
    QMediaPlayer::State state() const;
    QSlider* volumebar = new QSlider(Qt::Orientation::Horizontal, this);//need to access in tomeo.cpp
    QPushButton* volumebutton = new QPushButton(this);
    QPushButton* fullscreenbutton = new QPushButton(this);
    int volume=0;

public slots:
    void setState(QMediaPlayer::State state);
    void setVolume(int v);

signals:
    void play();
    void pause();
    void stop();
    void size(bool);
    void toggleVolume(int);


private slots:
    void StateChange();
    void StateChange1();
    void volumeChange();
    void SizeChange();


private:
    QMediaPlayer::State playerState = QMediaPlayer::StoppedState;
    QPushButton* playButton = new QPushButton(this);
    QPushButton* stopButton = new QPushButton(this);
};

#endif // PLAYERCONTROLS_H
