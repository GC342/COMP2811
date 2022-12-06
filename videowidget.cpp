#include "videowidget.h"
#include <QKeyEvent>
#include <QMouseEvent>

VideoWidget::VideoWidget(QWidget *parent)
    : QVideoWidget(parent) {

}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    setFullScreen(!isFullScreen());
    event->accept();
}
