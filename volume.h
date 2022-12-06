//
// Created by sc19mta on 30/11/2020(lmao).
//

#ifndef VOLUME_BAR_H
#define VOLUME_BAR_H

#include <QImageReader>
#include <QPushButton>
#include <QSlider>

class VolumeBar;
class VolumeButton;

class VolumeButton : public QPushButton {

    Q_OBJECT

private:
    int m_volume{0};

public:
    VolumeButton(QWidget* parent) : QPushButton(parent) {
        setVolume(0);
    }

    ~VolumeButton(){}

public slots:
    void setVolume(int v) {
        if (v > 50) {
            // high volume icon
            setIcon(QIcon(":icons/vol_f.png"));
        }
        else if (v > 0) {
            // low volume icon
            setIcon(QIcon(":icons/vol_l.png"));
        }
        else {
            // muted volume icon
            setIcon(QIcon(":icons/vol_m.png"));
        }
        m_volume = v;
    }

    void clicked() {
        // flip the bits
        setVolume(0 - m_volume);
        emit toggleVolume(m_volume);
    }

signals:
    void toggleVolume(int);
};

#endif // VOLUME_BAR_H
