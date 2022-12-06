#include "playercontrols.h"
#include "the_player.h"
#include <QPushButton>
#include <QWidget>
#include <QLayout>
#include <QSlider>
#include <QMediaPlayer>



PlayerControls::PlayerControls(QWidget *parent) :QWidget(parent) {
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->setMargin(0);
    controlLayout->setSpacing(3);

    playButton->setIcon(QIcon(":icons/play.png"));
    playButton->connect(playButton, SIGNAL(released()), this, SLOT(StateChange()));

    stopButton->setIcon(QIcon(":icons/stop.png"));
    stopButton->connect(stopButton, SIGNAL(released()), this, SLOT(StateChange1()));

    fullscreenbutton->setIcon(QIcon(":icons/fullscreen.png"));
    fullscreenbutton->connect(fullscreenbutton, SIGNAL(released()),this, SLOT(SizeChange()));

    volumebutton->setIcon(QIcon(":icons/vol_m.png"));
    volumebutton->connect(volumebutton, SIGNAL(released()), this, SLOT(volumeChange()));

    volumebar->setMaximumWidth(100);
    volumebar->setRange(0, 100);
    volumebar->connect(volumebar, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

    controlLayout->addWidget(playButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(fullscreenbutton);
    controlLayout->addWidget(volumebutton);
    controlLayout->addWidget(volumebar);
    controlLayout->addStretch(1);
    setLayout(controlLayout);
}

void PlayerControls::setState(QMediaPlayer::State state) {
        playerState = state;
        switch (state) {
        case QMediaPlayer::StoppedState:
            playButton->setIcon(QIcon(":icons/play.png"));
            break;
        case QMediaPlayer::PlayingState:
            playButton->setIcon(QIcon(":icons/pause.png"));
            break;
        case QMediaPlayer::PausedState:
            playButton->setIcon(QIcon(":icons/play.png"));
            break;
        }
}

QMediaPlayer::State PlayerControls::state() const {
    return playerState;
}

void PlayerControls::StateChange() {
    switch (playerState) {
    case QMediaPlayer::StoppedState:
        emit play();
        break;
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

//dedicated to stop function
void PlayerControls::StateChange1() {
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit stop();
        emit pause();
        break;
    case QMediaPlayer::PlayingState:
        emit stop();
        emit pause();
        break;
    }
}



void PlayerControls::volumeChange() {
    emit toggleVolume(0-volume);
}

void PlayerControls::SizeChange() {
    emit size(true);
}


void PlayerControls::setVolume(int v) {
    if (v > 50) {
        // high volume icon
        volumebutton->setIcon(QIcon(":icons/vol_f.png"));
    }
    else if (v > 0) {
        // low volume icon
        volumebutton->setIcon(QIcon(":icons/vol_l.png"));
    }
    else {
        // muted volume icon
        volumebutton->setIcon(QIcon(":icons/vol_m.png"));
    }
    volume = v;

}





