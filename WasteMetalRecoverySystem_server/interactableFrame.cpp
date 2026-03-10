#include "interactableFrame.h"

interactableFrame::interactableFrame(QWidget *parent) : QFrame(parent)
{
    m_bgColor = QColor(245,245,245);
    animation = new QVariantAnimation(this);
    isPressed = false;
    connect(animation, &QVariantAnimation::valueChanged, [=](const QVariant &value){
            setBgColor(value.value<QColor>());
        });

    this->setCursor(Qt::PointingHandCursor);

}

interactableFrame::~interactableFrame()
{

}


void interactableFrame::enterEvent(QEvent *event)
{
    transition(QColor("#E1F5FE"), 300);
    QFrame::enterEvent(event);

//    this->setFrameShape(QFrame::Panel);
//    this->setFrameShadow(QFrame::Raised);
}

void interactableFrame::leaveEvent(QEvent *event)
{
    transition(QColor("#F5F5F5"), 300);
    QFrame::leaveEvent(event);
//    this->setFrameShape(QFrame::Box);
//    this->setFrameShadow(QFrame::Sunken);
}

void interactableFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPressed = true;
        transition(QColor("#B3E5FC"), 50);
    }
    QFrame::mousePressEvent(event);
}

void interactableFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isPressed) {
        isPressed = false;
        transition(QColor("#E1F5FE"), 150);
        emit clicked();
    }
}

void interactableFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(m_bgColor);

    if (underMouse() || isPressed) {
        painter.setPen(QPen(QColor("#2196F3"), 1));
    } else {
        painter.setPen(QPen(QColor("#CCCCCC"), 1));
    }

    painter.drawRect(this->rect());
    QFrame::paintEvent(event);
}

void interactableFrame::transition(const QColor &targetColor, int duration)
{
    animation->stop();
      animation->setDuration(duration);
    animation->setStartValue(m_bgColor);
    animation->setEndValue(targetColor);
    animation->start();
}
