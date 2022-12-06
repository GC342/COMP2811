/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QTimer>
#include <QVideoWidget>
#include <QScrollArea>

#include <QFileDialog>
#include <QTabWidget>

#include "the_player.h"
#include "the_button.h"
#include "playercontrols.h"
#include "seek_bar.h"
#include "volume.h"
#include "videowidget.h"
#include "upload.h"

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}



void setupVideoUploadPage(QWidget*& pUploadPageWidget, QWidget* parent) {
    if (!pUploadPageWidget) {
        pUploadPageWidget = new UploadWidget(parent);
    }
}

void setupThumbnails(ThePlayer* player, QWidget* parent, std::vector<TheButtonInfo>& videos,
                     QWidget*& pOutThumbWidget, QScrollArea*& pOutScrollArea) {
    // a row of buttons
    pOutThumbWidget = new QWidget(parent);
    pOutThumbWidget->setMinimumWidth(1500);
    // a list of the buttons
    std::vector<TheButton*> buttons;
    {
        // the buttons are arranged horizontally
        QHBoxLayout* thumbnailLayout = new QHBoxLayout();

        pOutThumbWidget->setLayout(thumbnailLayout);
        // create the four buttons
        for ( int i = 0; i < 5; i++ ) {
            TheButton *button = new TheButton(pOutThumbWidget);
            button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
            buttons.push_back(button);
            thumbnailLayout->addWidget(button);
            button->init(&videos.at(i));
        }
        // tell the player what buttons and videos are available
        player->setContent(&buttons, &videos);
    }

    pOutScrollArea = new QScrollArea();
    pOutScrollArea->setWidget(pOutThumbWidget);
    pOutScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pOutScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    pOutScrollArea->setWidgetResizable(true);
    pOutScrollArea->setMinimumHeight(180);
}

void setupControls(ThePlayer* player, QWidget* parent, PlayerControls*& pControls) {
    pControls = new PlayerControls(parent);
    auto control = pControls;

    control->setState(player->state());
    control->setVolume(player->volume());

    player->connect(control,SIGNAL(play()),player,SLOT(play()));
    player->connect(control,SIGNAL(pause()),player,SLOT(pause()));
    player->connect(control,SIGNAL(stop()),player,SLOT(stop()));
    player->connect(control->volumebar,SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    player->connect(control, SIGNAL(toggleVolume(int)), player, SLOT(setVolume(int)));

    player->connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),control,SLOT(setState(QMediaPlayer::State)));
    player->connect(player,SIGNAL(volumeChanged(int)),control,SLOT(setVolume(int)));
    player->connect(player,SIGNAL(volumeChanged(int)),control->volumebar,SLOT(setValue(int)));
}

void setupMainPage(ThePlayer* player, QWidget* parent, std::vector<TheButtonInfo>& videos) {
    // initialise these variables in their own functions because of 50 line limit..
    QWidget* thumbnailWidget = nullptr;
    QScrollArea* scrollArea = nullptr;
    SeekBar* seekBar = nullptr;
    PlayerControls* control = nullptr;

    setupThumbnails(player, parent, videos, thumbnailWidget, scrollArea);

    //move down here, place it above the control buttons
    // seek bar
    // todo: make this not stutter?
    seekBar = new SeekBar(parent);
    player->connect(player, &QMediaPlayer::durationChanged, seekBar, &SeekBar::SetRange);
    player->connect(player, &QMediaPlayer::positionChanged, seekBar, &SeekBar::setValue);
    seekBar->connect(seekBar, &SeekBar::valueChanged, player, &QMediaPlayer::setPosition);

    // controls
    setupControls(player, parent, control);

    // the widget that will show the video
    VideoWidget *videoWidget = new VideoWidget();
    player->setVideoOutput(videoWidget);

    videoWidget->setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    videoWidget->connect(control,SIGNAL(size(bool)),videoWidget,SLOT(setFullScreen(bool)));

    // create the main window and layout
    QVBoxLayout *top = new QVBoxLayout();
    parent->setLayout(top);
    parent->setWindowTitle("tomeo");
    parent->setMinimumSize(800, 680);


    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(seekBar);
    top->addWidget(control);
    top->addWidget(scrollArea);
}

int main(int argc, char *argv[]) {
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);
    QTabWidget* pTabWidget = new QTabWidget();
    pTabWidget->setWindowTitle("tomeo");
    pTabWidget->setMinimumSize(800, 680);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( std::string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;

    QWidget mainTab;
    pTabWidget->addTab(&mainTab, "Main");
    setupMainPage(player, &mainTab, videos);

    QWidget* uploadPageWidget = nullptr;
    setupVideoUploadPage(uploadPageWidget, pTabWidget);
    pTabWidget->addTab(uploadPageWidget, "Upload");

    // showtime!
    pTabWidget->show();

    // wait for the app to terminate
    return app.exec();
}
