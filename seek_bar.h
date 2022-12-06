//
// Created by sc19mta on 30/11/2020(lmao).
//

#ifndef CUSTOM_SEEK_BAR_H
#define CUSTOM_SEEK_BAR_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>

class SeekBar : public QSlider
{
private:
    // dont wanna spam setValue calls
    time_t prevSetValTime;
public:
    SeekBar(QWidget* parent) : QSlider(Qt::Orientation::Horizontal, parent)
    {
        prevSetValTime = time(nullptr);

        setTracking(false);
    }
    ~SeekBar(){}


public slots:
    void SetRange(qint64 range)
    {
        setRange(0, (int)range);
    }

    void SetValue(qint64 value)
    {
        setValue(value);
    }
signals:
    void ValueChanged(qint64);
    void SliderClicked ();
};

#endif // SEEK_BAR_H
